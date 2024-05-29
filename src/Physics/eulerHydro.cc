#include "hydro.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class EulerHydro : public Hydro<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> insideIds;
public:
    EulerHydro() {}

    EulerHydro(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList,constants,eos), grid(grid){
        VerifyEulerHydroFields(nodeList);

        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* u                  = nodeList->getField<double>("specificInternalEnergy");

        State<dim>* state = &this->state;
        state->template addField<Lin::Vector<dim>>(position);
        state->template addField<Lin::Vector<dim>>(velocity);
        state->template addField<double>(u);

        for(int i=0;i<grid->size();i++)
            if(!grid->onBoundary(i))
                insideIds.push_back(i);
    }

    ~EulerHydro() {}

    virtual void
    ZeroTimeInitialize() override {      
        NodeList* nodeList = this->nodeList;
        Field<Lin::Vector<dim>>* v  = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* rho          = nodeList->getField<double>("density");
        Field<double>* u            = nodeList->getField<double>("specificInternalEnergy");

        EOSLookup();
    }

    virtual void
    VerifyEulerHydroFields(NodeList* nodeList) {
        if (nodeList->getField<Lin::Vector<dim>>("position") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("position");
                
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        for(int i=0;i<position->size();++i) {
            position->setValue(i,grid->getPosition(i));
        }
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override{  
        // drhodt = - rho * del dot v
        // dvdt = Forces - (del P) / rho
        // dudt = -P/rho * del dot v
        
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* position   = initialState->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* velocity   = initialState->template getField<Lin::Vector<dim>>("velocity");
        Field<double>* u                    = initialState->template getField<double>("u");
        Field<double>* pressure             = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed           = nodeList->getField<double>("soundSpeed");

        for (int h=0; h<insideIds.size(); ++h) {
            int i = insideIds[h];
            std::vector<int> nbrs = grid->getNeighboringCells(i);
        }

    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;

        EOSLookup();
    }

    virtual void
    EOSLookup() {        
        NodeList* nodeList = this->nodeList;

        Field<double>* rho          = nodeList->getField<double>("density");
        Field<double>* u            = nodeList->getField<double>("specificInternalEnergy");
        Field<double>* pressure     = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed   = nodeList->getField<double>("soundSpeed");

        EquationOfState* eos = this->eos;
        eos->setPressure(pressure,rho,u);
        eos->setSoundSpeed(soundSpeed,rho,u);
    }

    double
    getCell(int i,int j) {
        int idx = grid->index(i,j,0);
        NodeList* nodeList = this->nodeList;
        Field<double>* pr = nodeList->getField<double>("pressure");
        return pr->getValue(idx);
    }


    virtual double 
    EstimateTimestep() const override {
        // dt = dh/vmax
        return 0; 
    }
};
