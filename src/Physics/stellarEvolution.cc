#include "physics.hh"
#include "../Mesh/stellarGrid1d.hh"
#include "../EOS/equationOfState.hh"
#include <cmath>
#include <vector>
#include <algorithm>

class StellarEvolution : public Physics<1> {
public:
    using Vector       = Lin::Vector<1>;
    using VectorField  = Field<Vector>;
    using ScalarField  = Field<double>;

    Mesh::StellarGrid1d* grid;
    EquationOfState* eos;

    StellarEvolution(Mesh::StellarGrid1d* grid, NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos)
        : Physics<1>(nodeList, constants), grid(grid), eos(eos) {

        for (const std::string& name : 
            {"pressure", "density", "specificInternalEnergy", "luminosity", "temperature"}) {
            if (nodeList->getField<double>(name) == nullptr)
                nodeList->insertField<double>(name);
        }

        for (const std::string& name : 
            {"specificInternalEnergy", "temperature", "luminosity"}) {
            ScalarField* field = nodeList->getField<double>(name);
            this->state.template addField<double>(field);
        }
    }

    void EvaluateDerivatives(const State<1>* initialState,
                             State<1>& deriv,
                             const double time,
                             const double dt) override {
        ScalarField* u     = initialState->template getField<double>("specificInternalEnergy");
        ScalarField* rho   = initialState->template getField<double>("density");
        ScalarField* dUdt  = deriv.template getField<double>("specificInternalEnergy");

        NodeList* nodeList = this->nodeList;
        PhysicalConstants& constants = this->constants;

        ScalarField T("temperature", u->size());
        eos->setTemperature(&T, rho, u);

        // Compute ε_nuc(T)
        std::vector<double> eps(u->size());
        for (int i = 0; i < u->size(); ++i)
            eps[i] = epsilonNuc(T[i]);

        // Integrate L from center
        std::vector<double> L(u->size(), 0.0);
        for (int i = 1; i < u->size(); ++i)
            L[i] = L[i-1] + eps[i-1] * grid->dm(i);

        // Compute dL/dm and du/dt
        for (int i = 1; i < u->size() - 1; ++i) {
            double dLdm = (L[i+1] - L[i-1]) / (grid->m[i+1] - grid->m[i-1]);
            double du_dt = eps[i] - dLdm;
            dUdt->setValue(i, du_dt);
        }

        dUdt->setValue(0, 0.0);
        dUdt->setValue(u->size() - 1, 0.0);
    }

    void FinalizeStep(const State<1>* finalState) override {
        NodeList* nodeList = this->nodeList;
        PhysicalConstants& constants = this->constants;

        ScalarField* fu = finalState->template getField<double>("specificInternalEnergy");
        ScalarField* u  = nodeList->getField<double>("specificInternalEnergy");

        u->copyValues(fu);

        #pragma omp parallel for
        for (int i = 0; i < u->size(); ++i)
            u->setValue(i, std::max((*u)[i], 1e-12));  // Prevent nonphysical negative energies

        ScalarField* rho = nodeList->getField<double>("density");
        ScalarField* T   = nodeList->getField<double>("temperature");
        ScalarField* P   = nodeList->getField<double>("pressure");

        eos->setTemperature(T, rho, u);
        eos->setPressure(P, rho, u);

        ComputeHydrostaticEquilibrium();  // Optional: maintain strict equilibrium
    }

    void ComputeHydrostaticEquilibrium() {
        NodeList* nodeList = this->nodeList;
        PhysicalConstants& constants = this->constants;

        ScalarField* P   = nodeList->getField<double>("pressure");
        ScalarField* rho = nodeList->getField<double>("density");

        const int nz = grid->nz;
        P->setValue(nz - 1, 0.0);  // Surface pressure boundary condition

        for (int i = nz - 2; i >= 0; --i) {
            double r_next = grid->r[i + 1];
            double dPdm = -constants.G() * grid->m[i + 1] / (4 * M_PI * std::pow(r_next, 4));
            double Pi = (*P)[i + 1] - dPdm * grid->dm(i);
            P->setValue(i, Pi);
        }
    }

    double epsilonNuc(double T) const {
        return 1e-5 * std::pow(T / 1e7, 6);  // Toy pp-chain scaling law
    }

    void ComputeLuminosity() {
        NodeList* nodeList = this->nodeList;
        PhysicalConstants& constants = this->constants;
        
        ScalarField* L = nodeList->getField<double>("luminosity");
        ScalarField* T = nodeList->getField<double>("temperature");

        (*L)[0] = 0.0;
        for (int i = 1; i < grid->nz; ++i) {
            double eps = epsilonNuc((*T)[i - 1]);
            (*L)[i] = (*L)[i - 1] + eps * grid->dm(i);
        }
    }
};
