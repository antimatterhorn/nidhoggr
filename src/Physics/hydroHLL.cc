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
        Field<double>* soundSpeed = nodeList->getField<double>("soundSpeed");
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");

        Field<UType<dim>> HLLU = Field<UType<dim>>("copyHLLU",initialState->size());
        Field<Lin::Vector<dim>> F0 = Field<Lin::Vector<dim>>("f0",initialState->size());
        Field<Lin::Vector<dim>> F1 = Field<Lin::Vector<dim>>("f1",initialState->size());
        Field<Lin::Vector<dim>> F2 = Field<Lin::Vector<dim>>("f2",initialState->size());

        Field<Lin::Vector<dim>> ep = Field<Lin::Vector<dim>>("ep",initialState->size());
        Field<Lin::Vector<dim>> em = Field<Lin::Vector<dim>>("em",initialState->size());
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
            Lin::Vector<dim> cs = Lin::Vector<dim>();
            F0.setValue(i,uu.getU1());
            std::array<double, dim> ff1;
            ff1.fill(0);
            for(int k=0;k<dim;++k) {
                ff1[k] = pow(uu.getU1().values[k],2)/uu.getU0() + Pr;
                cs.values[k] = soundSpeed->getValue(i);
            }
            F1.setValue(i,ff1);
            F2.setValue(i,uu.getU1()*(uu.getU2()+1.0)/uu.getU0());

            ep.setValue(i,velocity->getValue(i) + cs);
            em.setValue(i,velocity->getValue(i) - cs);
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
