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
    }

    ~HydroHLL() {}

    virtual void
    VerifyHLLFields(NodeList* nodeList) {
        int numNodes = nodeList->size();
        if (nodeList->getField<UType<dim>>("HLLU") == nullptr)
            nodeList->insertField<UType<dim>>("HLLU");
    }

    virtual void
    PreStepInitialize() override {
        NodeList* nodeList = this->nodeList;

        Field<double>* density              = nodeList->getField<double>("density");
        Field<double>* pressure             = nodeList->getField<double>("pressure");
        Field<double>* energy               = nodeList->getField<double>("specificInternalEnergy");
        Field<Lin::Vector<dim>>* velocity   = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<UType<dim>>* HLLU             = nodeList->getField<UType<dim>>("HLLU");

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
            HLLU->setValue(i,uu);
        }
    }

    virtual void
    EvaluateDerivatives(const State* initialState, State& deriv, const double t) override{  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        Field<UType<dim>> HLLU = Field<UType<dim>>("copyHLLU",initialState->size());
        Field<Lin::Vector<dim>> F0 = Field<Lin::Vector<dim>>("f0",initialState->size());
        Field<Lin::Vector<dim>> F1 = Field<Lin::Vector<dim>>("f1",initialState->size());
        Field<Lin::Vector<dim>> F2 = Field<Lin::Vector<dim>>("f2",initialState->size());

        Field<double>* pressure             = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed           = nodeList->getField<double>("soundSpeed");
        Field<Lin::Vector<dim>>* velocity   = nodeList->getField<Lin::Vector<dim>>("velocity");

        std::vector<Field<Lin::Vector<dim>>*> F;
        F.push_back(&F0);
        F.push_back(&F1);
        F.push_back(&F2);

        Field<Lin::Vector<dim>> ep = Field<Lin::Vector<dim>>("ep",initialState->size());
        Field<Lin::Vector<dim>> em = Field<Lin::Vector<dim>>("em",initialState->size());
        HLLU.copyValues(initialState);
        for (int j=0; j<insideIds.size(); ++j) {
            int i = insideIds[j];
            double Pr = pressure->getValue(i);

            UType<dim> uu = HLLU.getValue(i);

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

        for (int j=0; j<insideIds.size(); ++j) {
            int i = insideIds[j];
            std::vector<int> nbrs = grid->getNeighboringCells(i);
            
            std::array<double, 3> FluxP, FluxM; 
            std::array<Lin::Vector<dim>, 3> Lv; 
            
            for (int k=0; k<dim; ++k) {
                FluxP = getFlux(k,i,nbrs[2*k],initialState, ep, em, F);
                FluxM = getFlux(k,nbrs[2*k+1],i,initialState, ep, em, F);
                for (int s=0; s<3; ++s)
                    Lv[s][k] = (FluxM[s] - FluxP[s])/grid->spacing(k);
            } 
            // now i need to decide how to handle the different axes for Lv[0] and Lv[2]
            double Lv0,Lv2;
            for (int k=0; k<dim; ++k) {
                Lv0+=Lv[0][k];
                Lv2+=Lv[2][k];
            }
            UType<dim> Lvi = UType<dim>(Lv0,Lv[1],Lv2);   
            deriv.setValue(i,Lvi);    
        }
    }

    std::array<double, 3>
    getFlux(int axis, int i, int j, 
        const Field<UType<dim>>* initialState, Field<Lin::Vector<dim>>& ep, Field<Lin::Vector<dim>>& em, std::vector<Field<Lin::Vector<dim>>*>& F) {
            double epR,epL,emR,emL,ap,am;
            epR = 0.5*(ep.getValue(j)[axis]+ep.getValue(i)[axis]);
            epL = ep.getValue(i).x();
            emR = 0.5*(em.getValue(j)[axis]+em.getValue(i)[axis]);
            emL = em.getValue(i).x();

            ap = std::max(std::max(epL,epR),0.0);
            am = std::max(std::max(-emL,-emR),0.0); 

            std::array<double, 3> FL,FR,Flux;
            UType<dim> UL,UR;
            UL.setU0(initialState->getValue(i).getU0());
            UR.setU0(0.5*(UL.getU0()+initialState->getValue(j).getU0()));
            UL.setU1(initialState->getValue(i).getU1());
            UR.setU1(0.5*(UL.getU1()+initialState->getValue(j).getU1()));
            UL.setU2(initialState->getValue(i).getU2());
            UR.setU2(0.5*(UL.getU2()+initialState->getValue(j).getU2()));
            for (int k=0; k<3; ++k) {
                FL[k] = F[k]->getValue(i)[axis];
                FR[k] = 0.5*(F[k]->getValue(i)[axis]+F[k]->getValue(j)[axis]);
            }
            Flux[0] = (ap*FL[0] + am*FR[0] - ap*am*(UR.getU0()-UL.getU0()))/(ap+am); 
            Flux[1] = (ap*FL[1] + am*FR[1] - ap*am*(UR.getU1()-UL.getU1())[axis])/(ap+am); 
            Flux[2] = (ap*FL[2] + am*FR[2] - ap*am*(UR.getU2()-UL.getU2()))/(ap+am);

            return Flux;
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