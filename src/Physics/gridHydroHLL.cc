#include "hydro.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class GridHydroHLL : public Hydro<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> insideIds;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;
    
    GridHydroHLL() {}

    GridHydroHLL(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList, constants, eos), grid(grid) {
        std::cout << grid->size() << std::endl;
        VerifyHLLFields(nodeList);

        State<dim>* state = &this->state;

        VectorField* v  = nodeList->getField<Vector>("velocity");
        ScalarField* rho          = nodeList->getField<double>("density");
        ScalarField* u            = nodeList->getField<double>("specificInternalEnergy");

        state->template addField<Vector>(v);
        state->template addField<double>(rho);
        state->template addField<double>(u);

        for (int i = 0; i < grid->size(); i++) {
            if (!grid->onBoundary(i))
                insideIds.push_back(i);
        }
    }

    ~GridHydroHLL() {}

    virtual void 
    ZeroTimeInitialize() override {
        EOSLookup();
    }

    virtual void 
    VerifyHLLFields(NodeList* nodeList) {
        if (nodeList->getField<Vector>("position") == nullptr)
            nodeList->insertField<Vector>("position");
                
        VectorField* position = nodeList->getField<Vector>("position");
        for (int i = 0; i < position->size(); ++i) {
            position->setValue(i, grid->getPosition(i));
        }
    }

    virtual void 
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        ScalarField F0("f0", initialState->size());
        ScalarField F1("f1", initialState->size());
        ScalarField F2("f2", initialState->size());
        ScalarField F3("f3", initialState->size());
        ScalarField F4("f4", initialState->size());

        ScalarField U0("u0", initialState->size());
        ScalarField U1("u1", initialState->size());
        ScalarField U2("u2", initialState->size());
        ScalarField U3("u3", initialState->size());
        ScalarField U4("u4", initialState->size());

        VectorField ep("ep", initialState->size());
        VectorField em("em", initialState->size());

        /*
            rho             rho*vx
            rho*vx          rho*vx^2 + P
        U = rho*vy      F = rho*vx*vy
            rho*vz          rho*vx*vz
            E               vx*(E+P)
        */

        VectorField* v   = initialState->template getField<Vector>("velocity");
        ScalarField* rho = initialState->template getField<double>("density");
        ScalarField* en  = initialState->template getField<double>("specificInternalEnergy");

        ScalarField* pressure   = nodeList->getField<double>("pressure");
        ScalarField* soundSpeed = nodeList->getField<double>("soundSpeed");

        std::vector<ScalarField*> U = {&U0, &U1, &U2, &U3, &U4};
        std::vector<ScalarField*> F = {&F0, &F1, &F2, &F3, &F4};

        for (int i = 0; i<numNodes; ++i) {
            double den = rho->getValue(i);
            double Pr = pressure->getValue(i);
            Vector vel = v->getValue(i);
            double ui = en->getValue(i);
            Vector cs = soundSpeed->getValue(i)*Vector::one();
            double E = ui+0.5*vel.mag2();

            U0.setValue(i,den);
            U1.setValue(i,den*vel.x());
            U2.setValue(i,den*vel.y()); // vel.y() returns 0 if dim == 1
            U3.setValue(i,den*vel.z());
            U4.setValue(i,E);

            ep.setValue(i, vel + cs);
            em.setValue(i, vel - cs);

            F0.setValue(i,den*vel.x());
            F1.setValue(i,den*vel.x()*vel.x() + Pr);
            F2.setValue(i,den*vel.x()*vel.y());
            printf("vx = %3.4f, vy = %3.4f, den = %3.4f, F2 = %3.4f\n",vel.x(),vel.y(),den,den*vel.x()*vel.y());
            F3.setValue(i,den*vel.x()*vel.z());
            F4.setValue(i,vel.x()*(E + Pr));
        }

        VectorField* DvDt = deriv.template getField<Vector>("velocity");
        ScalarField* DrDt = deriv.template getField<double>("density");
        ScalarField* DuDt = deriv.template getField<double>("specificInternalEnergy");

        for (int i = 0; i < numNodes; ++i) {
            std::vector<int> nbrs = grid->getNeighboringCells(i);
            
            std::array<double, 5> FluxP, FluxM, Lv={0,0,0,0,0}; 
            std::array<Vector, 5> Lvv;

            for (int k = 0; k < dim; ++k) {
                FluxP = getFlux(k, i, nbrs[2 * k], U, ep, em, F);
                FluxM = getFlux(k, nbrs[2 * k + 1], i, U, ep, em, F);
                for (int s = 0; s < 5; ++s) {
                    Lvv[s][k] = (FluxM[s] - FluxP[s]) / grid->spacing(k);
                    Lv[s] += Lvv[s][k]; // i guess? i dunno if it's just a sum here, but it seems like it ought to be
                }
            }

            DrDt->setValue(i, Lv[0]);
            
            Vector vi = v->getValue(i);
            double vx = vi.x();
            double vy = vi.y();
            double vz = vi.z();
            double den = rho->getValue(i);
            double Pr = pressure->getValue(i);
         
            double dvxdt, dvydt, dvzdt;
            dvxdt = (Lv[1]-vx*Lv[0])/den;
            dvydt = (Lv[2]-vy*Lv[0])/den;  // this can never work!!!
            dvzdt = (Lv[3]-vz*Lv[0])/den;

            printf("L1 = %3.4f, L2 = %3.4f\n",Lv[1],Lv[2]);
            Vector dvdt = Vector();
            dvdt[0] = dvxdt;
            if (dim>1)
                dvdt[1] = dvydt;
            if (dim>2)
                dvdt[2] = dvzdt;
            DvDt->setValue(i,dvdt);

            DuDt->setValue(i,Lv[4]-(vx*dvxdt + vy*dvydt + vz*dvzdt));

        }

        // for (int h = 0; h < insideIds.size(); ++h) {
        //     int i = insideIds[h];
        //     std::vector<int> nbrs = grid->getNeighboringCells(i);

        //     std::array<double, 3> FluxP, FluxM; 
        //     std::array<Vector, 3> Lv;
        //     for (int k = 0; k < dim; ++k) {
        //         FluxP = getFlux(k, i, nbrs[2 * k], U0,U1,U2, ep, em, F);
        //         FluxM = getFlux(k, nbrs[2 * k + 1], i, U0,U1,U2, ep, em, F);
        //         for (int s = 0; s < 3; ++s)
        //             Lv[s][k] = (FluxM[s] - FluxP[s]) / grid->spacing(k);
        //     }

        //     double Lv0 = 0, Lv2 = 0;
        //     for (int k = 0; k < dim; ++k) {
        //         Lv0 += Lv[0][k];
        //         Lv2 += Lv[2][k];
        //     }
        //     Vector vi = v->getValue(i);
        //     double den = rho->getValue(i);
        //     double ui = u->getValue(i);


        //     drdt->setValue(i, Lv0);
        //     dvdt->setValue(i, (Lv[1]-vi*Lv0)/den);   // (Lv1 - v*drdt)/rho
        //     dudt->setValue(i, (Lv2 - ui*Lv0 - 0.5*vi.mag2()*Lv0 - den*vi*dvdt->getValue(i)));  // (Lv2 - u*drdt - 0.5v*v*drdt - rho*v*dvdt)/rho
        // }

    }

    std::array<double, 5> 
    getFlux(int axis, int i, int j, std::vector<ScalarField*>& U, 
        VectorField& ep, VectorField& em, 
        std::vector<ScalarField*>& F) {
        double epR, epL, emR, emL, ap, am;
        epR = 0.5 * (ep.getValue(j)[axis] + ep.getValue(i)[axis]);
        epL = ep.getValue(i)[axis];
        emR = 0.5 * (em.getValue(j)[axis] + em.getValue(i)[axis]);
        emL = em.getValue(i)[axis];

        ap = std::max({epL, epR, 0.0});
        am = std::max({-emL, -emR, 0.0}); 

        std::array<double, 5> FL, FR, Flux;
        std::array<double, 5> UL, UR;

        for (int k = 0; k < 5; ++k) {
            UL[k] = U[k]->getValue(i);
            UR[k] = 0.5*(U[k]->getValue(i) + U[k]->getValue(j));
            FL[k] = F[k]->getValue(i);
            FR[k] = 0.5 * (F[k]->getValue(i) + F[k]->getValue(j));

            Flux[k] = (ap * FL[k] + am * FR[k] - ap * am * (UR[k] - UL[k])) / (ap + am); 
        }

        return Flux;
    }

    virtual void 
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        ScalarField* fdensity       = finalState->template getField<double>("density");
        VectorField* fvelocity      = finalState->template getField<Lin::Vector<dim>>("velocity");
        ScalarField* fu             = finalState->template getField<double>("specificInternalEnergy");

        ScalarField* density        = nodeList->getField<double>("density");
        VectorField* velocity       = nodeList->getField<Lin::Vector<dim>>("velocity");
        ScalarField* u              = nodeList->getField<double>("specificInternalEnergy");

        density->copyValues(fdensity);
        velocity->copyValues(fvelocity);
        u->copyValues(fu);
        EOSLookup();
    }

    virtual void 
    EOSLookup() {        
        NodeList* nodeList = this->nodeList;

        ScalarField* rho = nodeList->getField<double>("density");
        ScalarField* u = nodeList->getField<double>("specificInternalEnergy");
        ScalarField* pressure = nodeList->getField<double>("pressure");
        ScalarField* soundSpeed = nodeList->getField<double>("soundSpeed");

        EquationOfState* eos = this->eos;
        eos->setPressure(pressure, rho, u);
        eos->setSoundSpeed(soundSpeed, rho, u);
    }

    double 
    getCell(int i, int j, std::string fieldName = "pressure") {
        int idx = grid->index(i, j, 0);
        NodeList* nodeList = this->nodeList;
        ScalarField* pr = nodeList->getField<double>(fieldName);
        return pr->getValue(idx);
    }
};
