#include "hydro.hh"
#include "../Mesh/grid.hh"
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
        
        // this->derivFields.push_back(nodeList->getField<double>("density"));
        // this->derivFields.push_back(nodeList->getField<Lin::Vector<dim>>("velocity"));
        // this->derivFields.push_back(nodeList->getField<double>("specificInternalEnergy"));
        this->derivFields.push_back(nodeList->getField<std::array<double, 3>>("HLLU"));
        this->derivFields.push_back(nodeList->getField<std::array<double, 3>>("HLLF"));

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
        /*
        thisZone.u[0] = rho
        thisZone.u[1] = rho*v
        thisZone.u[2] = rho*thisZone.etot
        u = thisZone.u

        thisZone.f[0] = u[1]
        thisZone.f[1] = u[1]*u[1]/u[0] + Pr
        thisZone.f[2] = u[1]*thisZone.htot
        
        # somes notes on F(U)
        # f[0] = u[1]
        # f[1] = u[1]*u[1]/u[0] + Pr
        # f[2] = u[1](u[2]+1)/u[0]
        
        
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

    ~HydroHLL() {}

    virtual void
    VerifyHLLFields(NodeList* nodeList) {
        int numNodes = nodeList->size();
        if (nodeList->getField<std::array<double, 3>>("HLLU") == nullptr)
            nodeList->insertField<std::array<double, 3>>("HLLU");
        if (nodeList->getField<std::array<double, 3>>("HLLF") == nullptr)
            nodeList->insertField<std::array<double, 3>>("HLLF");
    }

    virtual void
    EvaluateDerivatives(const Field<std::array<double, 3>>* initialState, Field<std::array<double, 3>>& deriv, const double t) override{  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        Field<double>* density = nodeList->getField<double>("density");
        Field<double>* pressure = nodeList->getField<double>("pressure");
        Field<double>* energy = nodeList->getField<double>("specificInternalEnergy");
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<std::array<double, 3>>* HLLU = nodeList->getField<std::array<double, 3>>("HLLU");
        Field<std::array<double, 3>>* HLLF = nodeList->getField<std::array<double, 3>>("HLLF");
        for (int j=0; j<insideIds.size(); ++j) {
            int i = insideIds[j];
            
            std::array<double, 3> ui;
            ui[0] = density->getValue(i);
            ui[1] = density->getValue(i) * velocity->getValue(i).x();
            ui[2] = density->getValue(i) * energy->getValue(i);
            HLLU->setValue(i,ui);

            // well shoot, this is only 1 way, and i need all three directions somehow
            // one way might be to make HLLU 5 doubles with u[1] -> vx, u[4] -> vy, u[5] -> vz

            std::array<double, 3> fi;
            fi[0] = ui[1];
            fi[1] = ui[1]*ui[1]/ui[0] + pressure->getValue(i);
            fi[2] = ui[1]*(ui[2]+1)/ui[0];
            HLLF->setValue(i,fi);
        }
    }

    virtual void
    EvaluateDerivatives(const Field<Lin::Vector<dim>>* initialState, Field<Lin::Vector<dim>>& deriv, const double t) override{  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
    }

};
