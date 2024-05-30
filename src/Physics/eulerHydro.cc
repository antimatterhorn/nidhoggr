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

        Field<double>* density            = nodeList->getField<double>("density");
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* u                  = nodeList->getField<double>("specificInternalEnergy");

        State<dim>* state = &this->state;
        state->template addField<double>(density);
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

        Field<Lin::Vector<dim>>* velocity   = initialState->template getField<Lin::Vector<dim>>("velocity");
        Field<double>* density              = initialState->template getField<double>("density");
        Field<double>* drhodt               = deriv.template getField<double>("density");
        Field<double>* dudt                 = deriv.template getField<double>("specificInternalEnergy");
        Field<Lin::Vector<dim>>* dvdt       = deriv.template getField<Lin::Vector<dim>>("velocity");
        Field<double>* pressure             = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed           = nodeList->getField<double>("soundSpeed");

        for (int h=0; h<insideIds.size(); ++h) {
            int i = insideIds[h];
            std::vector<int> nbrs = grid->getNeighboringCells(i);

            double delDotV;
            double dvdx = velocity->getValue(nbrs[0])[0]-velocity->getValue(i)[0];
            dvdx += velocity->getValue(i)[0] - velocity->getValue(nbrs[1])[0];
            dvdx = dvdx*0.5/grid->dx;
            delDotV = dvdx;

            Lin::Vector<dim> delP;
            double dPdx = pressure->getValue(nbrs[0])-pressure->getValue(i);
            dPdx += pressure->getValue(i)-pressure->getValue(nbrs[1]);
            dPdx = dPdx*0.5/grid->dx;
            delP[0] = dPdx;

            if (dim > 1) {
                double dvdy = velocity->getValue(nbrs[2])[1]-velocity->getValue(i)[1];
                dvdy += velocity->getValue(i)[1] - velocity->getValue(nbrs[3])[1];
                dvdy = dvdy*0.5/grid->dy;
                delDotV += dvdy;

                double dPdy = pressure->getValue(nbrs[2])-pressure->getValue(i);
                dPdy += pressure->getValue(i)-pressure->getValue(nbrs[3]);
                dPdy = dPdy*0.5/grid->dy;
                delP[1] = dPdy;
            }              
            if (dim > 2) {
                double dvdz = velocity->getValue(nbrs[4])[2]-velocity->getValue(i)[2];
                dvdz += velocity->getValue(i)[2] - velocity->getValue(nbrs[5])[1];
                dvdz = dvdz*0.5/grid->dz;
                delDotV += dvdz;
                
                double dPdz = pressure->getValue(nbrs[4])-pressure->getValue(i);
                dPdz += pressure->getValue(i)-pressure->getValue(nbrs[5]);
                dPdz = dPdz*0.5/grid->dz;
                delP[2] = dPdz;
            }
            double P = pressure->getValue(i);
            double rho = density->getValue(i);
            drhodt->setValue(i,-rho*delDotV);
            dvdt->setValue(i,-delP/rho);
            dudt->setValue(i,-P/rho*delDotV);            
        }

    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;

        Field<double>* fdensity                 = finalState->template getField<double>("density");
        Field<Lin::Vector<dim>>* fvelocity      = finalState->template getField<Lin::Vector<dim>>("velocity");
        Field<double>* fu                       = finalState->template getField<double>("specificInternalEnergy");

        Field<double>* density                  = nodeList->getField<double>("density");
        Field<Lin::Vector<dim>>* velocity       = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* u                        = nodeList->getField<double>("specificInternalEnergy");

        density->copyValues(fdensity);
        velocity->copyValues(fvelocity);
        u->copyValues(fu);

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
