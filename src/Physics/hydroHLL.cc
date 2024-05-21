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

    HydroHLL(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList,constants,eos), grid(grid){
        std::cout << grid->size() << std::endl;
        VerifyHLLFields(nodeList);

        State<dim>* state = &this->state;

        Field<Lin::Vector<dim>>* u1 = nodeList->getField<Lin::Vector<dim>>("u1");       
        Field<double>* u0 = nodeList->getField<double>("u0");
        Field<double>* u2 = nodeList->getField<double>("u2");

        state->template addField<Lin::Vector<dim>>(u1);
        state->template addField<double>(u0);
        state->template addField<double>(u2);

        for(int i=0;i<grid->size();i++)
            if(!grid->onBoundary(i))
                insideIds.push_back(i);
    }

    ~HydroHLL() {}

    virtual void
    ZeroTimeInitialize() override {      
        NodeList* nodeList = this->nodeList;
        Field<Lin::Vector<dim>>* v  = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* rho          = nodeList->getField<double>("density");
        Field<double>* u            = nodeList->getField<double>("specificInternalEnergy");

        Field<Lin::Vector<dim>>* u1 = nodeList->getField<Lin::Vector<dim>>("u1");
        Field<double>* u0           = nodeList->getField<double>("u0");
        Field<double>* u2           = nodeList->getField<double>("u2");

        u0->copyValues(rho);

        for(int i=0;i<nodeList->size();++i){
            u1->setValue(i,rho->getValue(i)*v->getValue(i));
            u2->setValue(i,rho->getValue(i)*(0.5*v->getValue(i).mag2() + u->getValue(i)));
        }
        EOSLookup();
    }

    virtual void
    VerifyHLLFields(NodeList* nodeList) {
        if (nodeList->getField<double>("u0") == nullptr)
            nodeList->insertField<double>("u0");
        if (nodeList->getField<double>("u2") == nullptr)
            nodeList->insertField<double>("u2");
        if (nodeList->getField<Lin::Vector<dim>>("u1") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("u1");
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

        Field<Lin::Vector<dim>> F0      = Field<Lin::Vector<dim>>("f0",initialState->size());
        Field<Lin::Vector<dim>> F1      = Field<Lin::Vector<dim>>("f1",initialState->size());
        Field<Lin::Vector<dim>> F2      = Field<Lin::Vector<dim>>("f2",initialState->size());

        Field<Lin::Vector<dim>>* u1     = initialState->template getField<Lin::Vector<dim>>("u1");
        Field<double>* u0               = initialState->template getField<double>("u0");
        Field<double>* u2               = initialState->template getField<double>("u2");

        Field<Lin::Vector<dim>>* du1    = deriv.template getField<Lin::Vector<dim>>("u1");
        Field<double>* du0              = deriv.template getField<double>("u0");
        Field<double>* du2              = deriv.template getField<double>("u2");

        Field<double>* pressure         = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed       = nodeList->getField<double>("soundSpeed");

        // DO EOS LOOKUP FOR Pr and cs in finalize please

        std::vector<Field<Lin::Vector<dim>>*> F;
        F.push_back(&F0);
        F.push_back(&F1);
        F.push_back(&F2);

        Field<Lin::Vector<dim>> ep = Field<Lin::Vector<dim>>("ep",initialState->size());
        Field<Lin::Vector<dim>> em = Field<Lin::Vector<dim>>("em",initialState->size());

        for (int h=0; h<insideIds.size(); ++h) {
            int i = insideIds[h];

            Lin::Vector<dim> u1i    = u1->getValue(i);
            double u0i              = u0->getValue(i);
            double u2i              = u2->getValue(i);
            
            double Pr               = pressure->getValue(i);
            Lin::Vector<dim> cs     = Lin::Vector<dim>();
            Lin::Vector<dim> vi     = u1i/u0i;
            
            F0.setValue(i,u1i);
            
            std::array<double, dim> ff1;
            ff1.fill(0);
            for(int k=0;k<dim;++k) {
                ff1[k] = pow(u1i.values[k],2)/u0i + Pr;
                cs.values[k] = soundSpeed->getValue(i);
            }
            F1.setValue(i,ff1);
            F2.setValue(i,vi*(u2i+1.0));

            ep.setValue(i,vi + cs);
            em.setValue(i,vi - cs);
        }

        for (int h=0; h<insideIds.size(); ++h) {
            int i = insideIds[h];
            std::vector<int> nbrs = grid->getNeighboringCells(i);

            std::array<double, 3> FluxP, FluxM; 
            std::array<Lin::Vector<dim>, 3> Lv;
            for (int k=0; k<dim; ++k) {
                FluxP = getFlux(k,i,nbrs[2*k],initialState, ep, em, F);
                FluxM = getFlux(k,nbrs[2*k+1],i,initialState, ep, em, F);
                for (int s=0; s<3; ++s)
                    Lv[s][k] = (FluxM[s] - FluxP[s])/grid->spacing(k);
            }

            double Lv0,Lv2;
            for (int k=0; k<dim; ++k) {
                Lv0+=Lv[0][k];
                Lv2+=Lv[2][k];
            }
            du0->setValue(i,Lv0);
            du1->setValue(i,Lv[1]);
            du2->setValue(i,Lv2);
        }
    }

    std::array<double, 3>
    getFlux(int axis, int i, int j, 
        const State<dim>* initialState, 
        Field<Lin::Vector<dim>>& ep, Field<Lin::Vector<dim>>& em, 
        std::vector<Field<Lin::Vector<dim>>*>& F) {
            double epR,epL,emR,emL,ap,am;
            epR = 0.5*(ep.getValue(j)[axis]+ep.getValue(i)[axis]);
            epL = ep.getValue(i).x();
            emR = 0.5*(em.getValue(j)[axis]+em.getValue(i)[axis]);
            emL = em.getValue(i).x();

            ap = std::max(std::max(epL,epR),0.0);
            am = std::max(std::max(-emL,-emR),0.0); 

            std::array<double, 3> FL,FR,Flux;

            double u0L,u0R,u2L,u2R;
            Lin::Vector<dim> u1L,u1R;

            Field<Lin::Vector<dim>>* u1     = initialState->template getField<Lin::Vector<dim>>("u1");
            Field<double>* u0               = initialState->template getField<double>("u0");
            Field<double>* u2               = initialState->template getField<double>("u2");

            u0L = u0->getValue(i);
            u0R = 0.5*(u0->getValue(j) + u0->getValue(i));
            u1L = u1->getValue(i);
            u1R = 0.5*(u1->getValue(j) + u1->getValue(i));
            u2L = u2->getValue(i);
            u2R = 0.5*(u2->getValue(j) + u2->getValue(i));

            for (int k=0; k<3; ++k) {
                FL[k] = F[k]->getValue(i)[axis];
                FR[k] = 0.5*(F[k]->getValue(i)[axis]+F[k]->getValue(j)[axis]);
            }

            Flux[0] = (ap*FL[0] + am*FR[0] - ap*am*(u0R-u0L))/(ap+am); 
            Flux[1] = (ap*FL[1] + am*FR[1] - ap*am*(u1R-u1L)[axis])/(ap+am); 
            Flux[2] = (ap*FL[2] + am*FR[2] - ap*am*(u2R-u2L))/(ap+am);

            return Flux;
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;

        Field<Lin::Vector<dim>>* u1 = finalState->template getField<Lin::Vector<dim>>("u1");
        Field<double>* u0           = finalState->template getField<double>("u0");
        Field<double>* u2           = finalState->template getField<double>("u2");
        
        Field<Lin::Vector<dim>>* v  = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* rho          = nodeList->getField<double>("density");
        Field<double>* u            = nodeList->getField<double>("specificInternalEnergy");

        for(int i=0;i<nodeList->size();++i){
            double u0i              = u0->getValue(i);
            Lin::Vector<dim> u1i    = u1->getValue(i);
            double u2i              = u2->getValue(i);
            rho->setValue(i,u0i);
            v->setValue(i,u1i/u0i);
            u->setValue(i,u2i/u0i - 0.5*v->getValue(i).mag2());
        }

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

};
