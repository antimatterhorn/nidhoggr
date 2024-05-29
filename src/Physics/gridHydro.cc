#include "hydro.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class GridHydro : public Hydro<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> insideIds;
public:
    GridHydro() {}

    GridHydro(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList,constants,eos), grid(grid){
        std::cout << grid->size() << std::endl;
        VerifyGridHydroFields(nodeList);

        State<dim>* state = &this->state;


        for(int i=0;i<grid->size();i++)
            if(!grid->onBoundary(i))
                insideIds.push_back(i);
    }

    ~GridHydro() {}

    virtual void
    ZeroTimeInitialize() override {      
        NodeList* nodeList = this->nodeList;
        Field<Lin::Vector<dim>>* v  = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* rho          = nodeList->getField<double>("density");
        Field<double>* u            = nodeList->getField<double>("specificInternalEnergy");

        EOSLookup();
    }

    virtual void
    VerifyGridHydroFields(NodeList* nodeList) {
        if (nodeList->getField<Lin::Vector<dim>>("position") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("position");
                
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        for(int i=0;i<position->size();++i) {
            position->setValue(i,grid->getPosition(i));
        }
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override{  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();


        Field<double>* pressure         = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed       = nodeList->getField<double>("soundSpeed");

        Field<Lin::Vector<dim>> ep = Field<Lin::Vector<dim>>("ep",initialState->size());
        Field<Lin::Vector<dim>> em = Field<Lin::Vector<dim>>("em",initialState->size());

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

};
