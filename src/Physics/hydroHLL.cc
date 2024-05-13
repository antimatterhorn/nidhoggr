#include "hydro.hh"
#include "../Mesh/grid.hh"
#include "../Type/uType.hh"
#include <iostream>

template <int dim>
class HydroHLL : public Hydro<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> insideIds;
public:
    HydroHLL() {}

    HydroHLL(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList,constants){
        VerifyHLLFields(nodeList);

        this->derivFields.push_back(nodeList->getField<UType<dim>>("HLLU"));

        for(int i=0;i<grid->size();i++)
            if(!grid->onBoundary(i))
                insideIds.push_back(i);

        /*
        The grid neighbors list is in this order:
        left
        right
        up
        down
        front
        back
        */
    }

    ~HydroHLL() {}

    virtual void
    VerifyHLLFields(NodeList* nodeList) {
        int numNodes = nodeList->size();
        if (nodeList->getField<UType<dim>>("HLLU") == nullptr)
            nodeList->insertField<UType<dim>>("HLLU");
        if (nodeList->getField<std::array<double, 5>>("HLLF") == nullptr)
            nodeList->insertField<std::array<double, 5>>("HLLF");
    }

    virtual void
    EvaluateDerivatives(const Field<UType<dim>>* initialState, Field<UType<dim>>& deriv, const double t) override{  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        Field<double>* density = nodeList->getField<double>("density");
        Field<double>* pressure = nodeList->getField<double>("pressure");
        Field<double>* energy = nodeList->getField<double>("specificInternalEnergy");
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<UType<dim>> HLLU = Field<UType<dim>>("copyHLLU",initialState->size());
        HLLU.copyValues(initialState);
        for (int j=0; j<insideIds.size(); ++j) {
            int i = insideIds[j];
            
            double rho = density->getValue(i);
            double v = velocity->getValue(i).magnitude();
            double e = energy->getValue(i);
            double Pr = pressure->getValue(i);
            double etot = e+0.5*v*v;
            double htot = e+Pr/rho;

            Lin::Vector<dim> uv = rho*velocity->getValue(i);
            UType<dim> uu = UType<dim>(rho,uv,rho*etot);

            HLLU.setValue(i,uu);
            
            std::array<double, 3> ui,fi,gi,hi;
        
            // left-right first
            ui[0] = uu.getU0();
            ui[1] = uu.getU1().x();
            ui[2] = uu.getU2();
            fi[0] = ui[1];
            fi[1] = ui[1]*ui[1]/ui[0] + Pr;
            fi[2] = ui[1]*(ui[2]+1)/ui[0];
            if constexpr (dim > 1) {
                ui[1] = uu.getU1().y();
                gi[0] = ui[1];
                gi[1] = ui[1]*ui[1]/ui[0] + Pr;
                gi[2] = ui[1]*(ui[2]+1)/ui[0];
            }

            // i think i'm going to need to store these fluxes into the nodeList
            // so that i can access the neighbors' fluxes
        }
    }

    virtual void
    EvaluateDerivatives(const Field<Lin::Vector<dim>>* initialState, Field<Lin::Vector<dim>>& deriv, const double t) override{  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
    }

    virtual void
    FinalizeStep() override {
        NodeList* nodeList = this->nodeList;

        Field<double>* density = nodeList->getField<double>("density");
        Field<double>* pressure = nodeList->getField<double>("pressure");
        Field<double>* energy = nodeList->getField<double>("specificInternalEnergy");
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<UType<dim>>* HLLU = nodeList->getField<UType<dim>>("HLLU");
        for (int j=0; j<insideIds.size(); ++j) {
            int i = insideIds[j];
            UType<dim> ui = HLLU->getValue(i);
            double rho = ui.getU0();
            Lin::Vector<dim> v = ui.getU1()/rho;
            double e = ui.getU2()/ui.getU0() - 0.5*v.mag2();
            density->setValue(i,rho);   // max this with rhofloor
            energy->setValue(i,e);
            velocity->setValue(i,v);             
        }

    }

};
