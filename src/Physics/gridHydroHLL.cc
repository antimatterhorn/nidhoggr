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

    GridHydroHLL(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList, constants, eos), grid(grid) {
        std::cout << grid->size() << std::endl;
        VerifyHLLFields(nodeList);

        State<dim>* state = &this->state;

        VectorField* v            = nodeList->getField<Vector>("velocity");
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
        grid->assignPositions(nodeList);
    }

    virtual void 
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        VectorField F0("f0", initialState->size());
        VectorField F1("f1", initialState->size());
        VectorField F2("f2", initialState->size());

        ScalarField u0("u0", initialState->size());
        VectorField u1("u1", initialState->size());
        ScalarField u2("u2", initialState->size());

        VectorField* v   = initialState->template getField<Vector>("velocity");
        ScalarField* rho = initialState->template getField<double>("density");
        ScalarField* u   = initialState->template getField<double>("specificInternalEnergy");

        u0.copyValues(rho);

        VectorField* dvdt = deriv.template getField<Vector>("velocity");
        ScalarField* drdt = deriv.template getField<double>("density");
        ScalarField* dudt = deriv.template getField<double>("specificInternalEnergy");

        ScalarField* pressure   = nodeList->getField<double>("pressure");
        ScalarField* soundSpeed = nodeList->getField<double>("soundSpeed");

        std::vector<VectorField*> F = {&F0, &F1, &F2};

        VectorField ep("ep", initialState->size());
        VectorField em("em", initialState->size());

        for (int h = 0; h < insideIds.size(); ++h) {
            int i = insideIds[h];

            double Pr = pressure->getValue(i);
            double den = rho->getValue(i);
            Vector vi = v->getValue(i);
            double e = u->getValue(i) + 0.5*vi.mag2();         // u + 1/2 v^2
            Vector cs = Vector::one()*soundSpeed->getValue(i);

            u1.setValue(i,den*vi);
            u2.setValue(i,den*e);

            F0.setValue(i,den*vi);                             // rho*v
            F1.setValue(i,Vector::one()*(den*vi.mag2()+Pr));   // rho*v^2 + Pr
            F2.setValue(i,vi*(den*e+1));                       // v*(rho*e+1)

            ep.setValue(i, vi + cs);
            em.setValue(i, vi - cs);
        }

        for (int h = 0; h < insideIds.size(); ++h) {
            int i = insideIds[h];
            std::vector<int> nbrs = grid->getNeighboringCells(i);

            std::array<double, 3> FluxP, FluxM; 
            std::array<Vector, 3> Lv;
            for (int k = 0; k < dim; ++k) {
                FluxP = getFlux(k, i, nbrs[2 * k], u0,u1,u2, ep, em, F);
                FluxM = getFlux(k, nbrs[2 * k + 1], i, u0,u1,u2, ep, em, F);
                for (int s = 0; s < 3; ++s)
                    Lv[s][k] = (FluxM[s] - FluxP[s]) / grid->spacing(k);
            }

            double Lv0 = 0, Lv2 = 0;
            for (int k = 0; k < dim; ++k) {
                Lv0 += Lv[0][k];
                Lv2 += Lv[2][k];
            }
            Vector vi = v->getValue(i);
            double den = rho->getValue(i);
            double ui = u->getValue(i);

            drdt->setValue(i, Lv0);
            dvdt->setValue(i, (Lv[1]-vi*Lv0)/den);   // (Lv1 - v*drdt)/rho
            dudt->setValue(i, (Lv2 - ui*Lv0 - 0.5*vi.mag2()*Lv0 - den*vi*dvdt->getValue(i)));  // (Lv2 - u*drdt - 0.5v*v*drdt - rho*v*dvdt)/rho
        }

    }

    std::array<double, 3> 
    getFlux(int axis, int i, int j, ScalarField& u0, VectorField& u1, ScalarField& u2, 
        VectorField& ep, VectorField& em, 
        std::vector<VectorField*>& F) {
        double epR, epL, emR, emL, ap, am;
        epR = 0.5 * (ep.getValue(j)[axis] + ep.getValue(i)[axis]);
        epL = ep.getValue(i)[axis];
        emR = 0.5 * (em.getValue(j)[axis] + em.getValue(i)[axis]);
        emL = em.getValue(i)[axis];

        ap = std::max({epL, epR, 0.0});
        am = std::max({-emL, -emR, 0.0}); 

        std::array<double, 3> FL, FR, Flux;

        double u0L, u0R, u2L, u2R;
        Vector u1L, u1R;

        u0L = u0.getValue(i);
        u0R = 0.5 * (u0.getValue(j) + u0.getValue(i));
        u1L = u1.getValue(i);
        u1R = 0.5 * (u1.getValue(j) + u1.getValue(i));
        u2L = u2.getValue(i);
        u2R = 0.5 * (u2.getValue(j) + u2.getValue(i));

        for (int k = 0; k < 3; ++k) {
            FL[k] = F[k]->getValue(i)[axis];
            FR[k] = 0.5 * (F[k]->getValue(i)[axis] + F[k]->getValue(j)[axis]);
        }

        Flux[0] = (ap * FL[0] + am * FR[0] - ap * am * (u0R - u0L)) / (ap + am); 
        Flux[1] = (ap * FL[1] + am * FR[1] - ap * am * (u1R - u1L)[axis]) / (ap + am); 
        Flux[2] = (ap * FL[2] + am * FR[2] - ap * am * (u2R - u2L)) / (ap + am);

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
