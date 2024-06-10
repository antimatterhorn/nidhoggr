#include "physics.hh"
#include "../Mesh/grid.hh"
#include "../IO/importDepthMap.hh"
#include <iostream>

template <int dim>
class WaveEquation : public Physics<dim> {
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

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>* grid, double C) : 
        Physics<dim>(nodeList,constants),
        grid(grid), C(C) {
        VerifyWaveFields();

        grid->assignPositions(nodeList);

        ScalarField* cs = nodeList->getField<double>("soundSpeed");
        for (int i=0; i<nodeList->getNumNodes();++i) cs->setValue(i,C);
    }

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<2>* grid, const std::string& depthMap) : 
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

    ~WaveEquation() {}

    void
    VerifyWaveFields() {
        NodeList* nodeList = this->nodeList;
        if (nodeList->getField<double>("phi") == nullptr)
            nodeList->insertField<double>("phi");
        if (nodeList->getField<double>("xi") == nullptr)
            nodeList->insertField<double>("xi");
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
            double c = cs->getValue(i);
            std::vector<int> neighbors = (ocean ? grid2d->getNeighboringCells(i) : grid->getNeighboringCells(i));
            double laplace2 = -4*phi->getValue(i);
            for (auto idx : neighbors) {
                laplace2 += phi->getValue(idx);
            }                
            laplace2 = (ocean ? laplace2/pow(grid2d->dx,2.0) : laplace2/pow(grid->dx,2.0));
            DxiDt->setValue(i,laplace2*c*c); 
            DphiDt->setValue(i,dt*DxiDt->getValue(i)+xi->getValue(i));
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

        xi->copyValues(fxi);
        phi->copyValues(fphi);
    }

    virtual double 
    EstimateTimestep() const override { 
        double dx = (ocean ? grid2d->dx : grid->dx);
        double cfl = 0.1;
        return cfl/C*dx;
    }


// d^2 phi / dt^2 = c^2 del^2 phi

// IIRC, which solution you get all comes down to initial conditions
// and boundary conditions.

// RHS is second spatial derivative.  Easy.  From Taylor,
// phi_{i+1,j} = phi_ij + dx * phi,x_ij + dx^2/2 phi,,x_ij + ... // where ,,x means d^2/dx^2
// phi_{i-1,j} = phi_ij - dx * phi,x_ij + dx^2/2 phi,,x_ij + ...
// phi_{i,j+1} = phi_ij + dy * phi,y_ij + dy^2/2 phi,,y_ij + ... // where ,,y means d^2/dy^2
// phi_{i,j-1} = phi_ij - dy * phi,y_ij + dy^2/2 phi,,y_ij + ...
// --------- = -----------------------------------
//           = 4 * phi_ij          + 2 * dx^2/2 phi,,x_ij + 2 * dy^2/2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 phi,,x_ij + dy^2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 (phi,,x_ij + phi,,y_ij) + ... // assume dx=dy
//           = 4 * phi_ij          + dx^2 del^2(phi_ij) + ... // assume dx=dy

// del^2 phi_ij = (-4*phi_{i,j} + phi_{i+1,j} + phi_{i-1,j} phi_{i,j+1} + phi_{i,j-1})/dx^2

};
