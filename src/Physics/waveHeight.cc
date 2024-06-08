#include "physics.hh"
#include "../Mesh/grid.hh"
#include "../IO/importDepthMap.hh"
#include <iostream>

class WaveHeight : public Physics<2> {
protected:
    Mesh::Grid<2>* grid;
    double dtmin;
    double g;
    double maxC;
public:
    using Vector = Lin::Vector<2>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    WaveHeight(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<2>* grid, std::string& depthMap) : 
        Physics<2>(nodeList,constants),
        grid(grid), g(constants.ESurfaceGrav()) {
        VerifyWaveFields();

        if (nodeList->getField<double>("depth") == nullptr)
            nodeList->insertField<double>("depth");

        grid->insertField<double>("depth");
        ImportDepthMap map(depthMap);
        map.populateDepthField(grid);

        ScalarField* depth      = grid->template getField<double>("depth");
        ScalarField* nodeDepth  = nodeList->getField<double>("depth");
        nodeDepth->copyValues(depth);

        ScalarField* cs = nodeList->getField<double>("soundSpeed");
        maxC = 0;
        #pragma omp parallel for reduction(max:maxC)
        for (int i=0; i<nodeList->getNumNodes();++i) {
            double c = (depth->getValue(i) < 0 ? sqrt(g*std::abs(depth->getValue(i))) : 0);
            cs->setValue(i,c);
            maxC = std::max(c,maxC);
        }
    }

    ~WaveHeight() {}

    void
    VerifyWaveFields() {
        NodeList* nodeList = this->nodeList;
        if (nodeList->getField<double>("phi") == nullptr)
            nodeList->insertField<double>("phi");
        if (nodeList->getField<double>("xi") == nullptr)
            nodeList->insertField<double>("xi");
        if (nodeList->getField<double>("soundSpeed") == nullptr)
            nodeList->insertField<double>("soundSpeed");
        
        grid->assignPositions(nodeList);

        State<2>* state = &this->state;
        ScalarField* xi = nodeList->getField<double>("xi");
        state->template addField<double>(xi);
        ScalarField* phi = nodeList->getField<double>("phi");
        state->template addField<double>(phi);
    }

    virtual void
    PreStepInitialize() override {
        State<2> state      = this->state;
        NodeList* nodeList  = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<2>* initialState, State<2>& deriv, const double time, const double dt) override {  
        NodeList* nodeList  = this->nodeList;
        int numNodes        = nodeList->size();
        
        ScalarField* xi     = initialState->template getField<double>("xi");
        ScalarField* phi    = initialState->template getField<double>("phi");

        ScalarField* DxiDt  = deriv.template getField<double>("xi");
        ScalarField* DphiDt = deriv.template getField<double>("phi");

        ScalarField* cs     = nodeList->getField<double>("soundSpeed");
        
        #pragma omp parallel for
        for (int i=0; i<numNodes; ++i) {
            double C = cs->getValue(i);
            std::vector<int> neighbors = grid->getNeighboringCells(i);
            double laplace2 = -4*phi->getValue(i);
            for (auto idx : neighbors) {
                laplace2 += phi->getValue(idx);
            }                
            laplace2 = laplace2/pow(grid->dx,2.0);
            DxiDt->setValue(i,laplace2*C*C); 
            DphiDt->setValue(i,dt*DxiDt->getValue(i)+xi->getValue(i));         
        }
    }

    double
    getCell(int i,int j, std::string fieldName="phi") {
        int idx             = grid->index(j,i,0);
        NodeList* nodeList  = this->nodeList;
        ScalarField* phi    = nodeList->getField<double>(fieldName);
        return phi->getValue(idx);
    }

    virtual void
    FinalizeStep(const State<2>* finalState) override {
        NodeList* nodeList  = this->nodeList;

        ScalarField* fxi    = finalState->template getField<double>("xi");
        ScalarField* fphi   = finalState->template getField<double>("phi");

        ScalarField* xi     = nodeList->getField<double>("xi");
        ScalarField* phi    = nodeList->getField<double>("phi");

        xi->copyValues(fxi);
        phi->copyValues(fphi);
    }

    virtual double 
    EstimateTimestep() const override { 
        double dx = grid->dx;
        double cfl = 0.1;
        return cfl/maxC*dx;
    }
};