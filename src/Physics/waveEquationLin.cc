#include "physics.hh"
#include "../Mesh/grid.hh"
#include "../IO/importDepthMap.hh"
#include <iostream>

template <int dim>
class WaveEquationLinear : public Physics<dim> {
protected:
    Mesh::Grid<dim>* grid;
    Mesh::Grid<2>* grid2d;
    bool ocean = false;
    double C;
    double dtmin;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    WaveEquationLinear(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>* grid, double C) : 
        Physics<dim>(nodeList,constants),
        grid(grid), C(C) {
        VerifyWaveFields();

        grid->assignPositions(nodeList);

        ScalarField* cs = nodeList->getField<double>("soundSpeed");
        for (int i=0; i<nodeList->getNumNodes();++i) cs->setValue(i,C);
    }

    WaveEquationLinear(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<2>* grid, const std::string& depthMap) : 
        Physics<dim>(nodeList, constants),
        grid2d(grid), C(constants.ESurfaceGrav()), ocean(true) {
        if (dim != 2) {
            std::cerr << "Error: This constructor can only be used with dim = 2" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        VerifyWaveFields();

        grid2d->assignPositions(nodeList);

        if (nodeList->getField<double>("depth") == nullptr)
            nodeList->insertField<double>("depth");

        grid2d->template insertField<double>("depth");
        ImportDepthMap map(depthMap);
        map.populateDepthField(grid2d);

        ScalarField* depth      = grid2d->template getField<double>("depth");
        ScalarField* nodeDepth  = nodeList->getField<double>("depth");
        nodeDepth->copyValues(depth);

        ScalarField* cs = nodeList->getField<double>("soundSpeed");
        double maxC = 0;
        #pragma omp parallel for reduction(max:maxC)
        for (int i=0; i<nodeList->getNumNodes();++i) {
            double c = (depth->getValue(i) < 0 ? sqrt(C*std::abs(depth->getValue(i))) : 0);
            cs->setValue(i,c);
            maxC = std::max(c,maxC);
        }
        C = maxC;
    }

    ~WaveEquationLinear() {}

    void
    VerifyWaveFields() {
        NodeList* nodeList = this->nodeList;
        if (nodeList->getField<double>("phi") == nullptr)
            nodeList->insertField<double>("phi");
        if (nodeList->getField<double>("xi") == nullptr)
            nodeList->insertField<double>("xi");
        if (nodeList->getField<double>("maxphi") == nullptr)
            nodeList->insertField<double>("maxphi");
        if (nodeList->getField<double>("soundSpeed") == nullptr)
            nodeList->insertField<double>("soundSpeed");       

        ScalarField* xi     = nodeList->getField<double>("xi");
        ScalarField* phi    = nodeList->getField<double>("phi");

        State<dim>* state = &this->state;
        state->template addField<double>(xi);
        state->template addField<double>(phi);
    }

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        
        ScalarField* xi     = initialState->template getField<double>("xi");
        ScalarField* phi    = initialState->template getField<double>("phi");

        ScalarField* DxiDt  = deriv.template getField<double>("xi");
        ScalarField* DphiDt = deriv.template getField<double>("phi");

        ScalarField* cs     = nodeList->getField<double>("soundSpeed");

        #pragma omp parallel for 
        for (int i=0; i<numNodes; ++i) {
            if (!grid->onBoundary(i)) {
                double c = cs->getValue(i);
                std::vector<int> neighbors = (ocean ? grid2d->getNeighboringCells(i) : grid->getNeighboringCells(i));
                double DuDx = 0.0;
                double DvDx = 0.0;
                for (auto idx : neighbors) {
                    // Compute the difference between the value at the current index and the neighbor index
                    DuDx += phi->getValue(idx) - phi->getValue(i);
                    DvDx += xi->getValue(idx) - xi->getValue(i);
                }

                // Normalize by the grid spacing
                DuDx = (ocean ? DuDx/grid2d->dx : DuDx/grid->dx);
                DvDx = (ocean ? DvDx/grid2d->dx : DvDx/grid->dx);
                DxiDt->setValue(i,c*DvDx); 
                DphiDt->setValue(i,xi->getValue(i) - c*DuDx);
            }
        }
    }

    double
    getCell(int i,int j, std::string fieldName="phi") {
        int idx = (ocean ? grid2d->index(j,i,0) : grid->index(j,i,0));
        NodeList* nodeList  = this->nodeList;
        ScalarField* phi    = nodeList->getField<double>(fieldName);
        return phi->getValue(idx);
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        ScalarField* fxi    = finalState->template getField<double>("xi");
        ScalarField* fphi   = finalState->template getField<double>("phi");

        ScalarField* xi     = nodeList->getField<double>("xi");
        ScalarField* phi    = nodeList->getField<double>("phi");
        ScalarField* mphi   = nodeList->getField<double>("maxphi");

        xi->copyValues(fxi);
        phi->copyValues(fphi);

        for (int i=0; i<numNodes; ++i)
            mphi->setValue(i,std::max(mphi->getValue(i),std::abs(phi->getValue(i))));
    }

    virtual double 
    EstimateTimestep() const override { 
        double dx = (ocean ? grid2d->dx : grid->dx);
        double cfl = 0.1;
        return cfl/C*dx;
    }

/*
u = phi
v = xi

d^2 u / dt^2 - c^2 del^2 u = 0
(d/dt - c d/dx)(d/dt + c d/dx) u = 0

du/dt + c du/dx = v
dv/dt - c dv/dx = 0

du/dt = v - c du/dx
dv/dt = c dv/dx

*/
};
