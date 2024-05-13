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
        right
        left
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

        std::vector<Field<Lin::Vector<dim>>*> F;
        F.push_back(&F0);
        F.push_back(&F1);
        F.push_back(&F2);

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
            
            std::array<double, 3> FluxP = getFluxLeftRight(i,nbrs[0],initialState, ep, em, F);
            std::array<double, 3> FluxM = getFluxLeftRight(nbrs[1],i,initialState, ep, em, F);
            std::array<double, 3> Lv;
            for (int k=0; k<3; ++k)
                Lv[k] = FluxM[k] - FluxP[k];
            UType<dim> dui = UType<dim>();
        }
        /*
        solve Fi+1/2
        so L = i and R = i+1
        then L=i-1 and R=i

        for i in range(1,nzones-1):
        # solve Fi+1/2
        # so L = i and R = i-1
        epR = 0.5*(zones[i+1].ep + zones[i].ep)
        epL = zones[i].ep
        emR = 0.5*(zones[i+1].em + zones[i].em)
        emL = zones[i].em

        ap = max(epL,epR,0)
        am = max(-emL,-emR,0)

        for j in range(3):
            fL[j] = zones[i].f[j]
            fR[j] = 0.5*(zones[i+1].f[j] + fL[j])
            uL[j] = zones[i].u[j]
            uR[j] = 0.5*(zones[i+1].u[j] + uL[j])
            flp[j] = (ap*fL[j] + am*fR[j] - ap*am*(uR[j]-uL[j]))/(ap+am)
        # solve Fi-1/2
        # so L = i-1 and R = i
        epR = epL
        emR = emL
        epL = 0.5*(zones[i-1].ep + epR)
        emL = 0.5*(zones[i-1].em + emR)

        ap = max(epL,epR,0)
        am = max(-emL,-emR,0)

        for j in range(3):
            fR[j] = fL[j]
            uR[j] = uL[j]
            fL[j] = 0.5*(zones[i-1].f[j] + fR[j])
            uL[j] = 0.5*(zones[i-1].u[j] + uR[j])
            flm[j] = (ap*fL[j] + am*fR[j] - ap*am*(uR[j]-uL[j]))/(ap+am)
            Lv[j] = (flm[j] - flp[j])
            uZones[i].u[j] += dt*Lv[j]/dx
        */
    }


    std::array<double, 3>
    getFluxLeftRight(int i, int j, 
        const Field<UType<dim>>* initialState, Field<Lin::Vector<dim>>& ep, Field<Lin::Vector<dim>>& em, std::vector<Field<Lin::Vector<dim>>*>& F) {
            double epR,epL,emR,emL,ap,am;
            epR = 0.5*(ep.getValue(j).x()+ep.getValue(i).x());
            epL = ep.getValue(i).x();
            emR = 0.5*(em.getValue(j).x()+em.getValue(i).x());
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
                FL[k] = F[k]->getValue(i).x();
                FR[k] = 0.5*(F[k]->getValue(i).x()+F[k]->getValue(j).x());
            }
            Flux[0] = (ap*FL[0] + am*FR[0] - ap*am*(UR.getU0()-UL.getU0()))/(ap+am); 
            Flux[1] = (ap*FL[1] + am*FR[1] - ap*am*(UR.getU1()-UL.getU1()).x())/(ap+am); 
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
