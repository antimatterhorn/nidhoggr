#include "physics.hh"
#include "../EOS/equationOfState.hh"
#include <cmath>
#include <vector>
#include <algorithm>

class StellarEvolution : public Physics<1> {
private:
    double dm;
    int numZones;
public:
    using Vector       = Lin::Vector<1>;
    using VectorField  = Field<Vector>;
    using ScalarField  = Field<double>;

    EquationOfState* eos;
    double totalMass;
    double centralTemperature;

    StellarEvolution(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, 
        double totalMass, double centralTemperature)
        : Physics<1>(nodeList, constants), numZones(nodeList->size()), eos(eos),
        totalMass(totalMass), centralTemperature(centralTemperature) {

        for (const std::string& name : 
            {"pressure", "density", "mass", "radius", "specificInternalEnergy", "luminosity", "temperature"}) {
            if (nodeList->getField<double>(name) == nullptr)
                nodeList->insertField<double>(name);
        }
        
        for (const std::string& name : 
            {"specificInternalEnergy", "temperature", "luminosity"}) {
            ScalarField* field = nodeList->getField<double>(name);
            this->state.template addField<double>(field);
        }

        dm = totalMass/numZones;

        BuildHydrostaticModel();
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
        
        ScalarField T("temperature", u->size()); // local storage of temperature for computing derivatives
        ScalarField* m = nodeList->getField<double>("mass");

        eos->setTemperature(&T, rho, u);
        
        // Compute ε_nuc(T)
        std::vector<double> eps(u->size());
        for (int i = 0; i < u->size(); ++i)
            eps[i] = epsilonNuc(T[i]);

        // Integrate L from center
        std::vector<double> L(u->size(), 0.0);
        for (int i = 1; i < u->size(); ++i)
            L[i] = L[i-1] + eps[i-1] * dm;

        // Compute dL/dm and du/dt
        for (int i = 1; i < u->size() - 1; ++i) {
            double dLdm = (L[i+1] - L[i-1]) / (m->getValue(i+1) - m->getValue(i-1));
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
        ScalarField* m   = nodeList->getField<double>("mass");
        ScalarField* r   = nodeList->getField<double>("radius");

        const int nz = m->size();
        P->setValue(nz - 1, 0.0);  // Surface pressure boundary condition

        for (int i = nz - 2; i >= 0; --i) {
            double r_next = r->getValue(i+1);
            double dPdm = -constants.G() * m->getValue(i+1) / (4 * M_PI * std::pow(r_next, 4));
            double Pi = (*P)[i + 1] - dPdm *dm;
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
        for (int i = 1; i < numZones; ++i) {
            double eps = epsilonNuc((*T)[i - 1]);
            (*L)[i] = (*L)[i - 1] + eps *dm;
        }
    }

    void BuildHydrostaticModel() {
        NodeList* nodeList = this->nodeList;
        PhysicalConstants& constants = this->constants;

        const int nz = numZones;

        std::vector<double> m(nz), r(nz), rho(nz), u(nz), P(nz), T(nz);
        m[0] = 0.5 * dm;
        r[0] = 1e-5;

        // Step 1: compute u from T using EOS
        double tmpU;
        double tmpT;
        double tmpRho;

        T[0] = centralTemperature;
        tmpT    = T[0];
        tmpRho  = 1;

        eos->setInternalEnergyFromTemperature(&tmpU, &tmpRho, &tmpT);
        u[0] = tmpU;

        // Newton-Raphson solve for rho[0]
        double rho_guess = tmpRho;
        const double tol = 1e-8;
        for (int iter = 0; iter < 20; ++iter) {
            tmpRho = rho_guess;
            eos->setPressure(&tmpU, &tmpRho, &tmpU);
            double P_rho = tmpU;
            double targetP = P_rho;

            tmpRho = rho_guess + 1e-6 * rho_guess;
            eos->setPressure(&tmpU, &tmpRho, &tmpU);
            double P_plus = tmpU;

            double df = (P_plus - P_rho) / (1e-6 * rho_guess);
            double delta = (P_rho - targetP) / df;
            rho_guess -= delta;

            if (std::abs(delta / rho_guess) < tol) break;
        }

        rho[0] = rho_guess;

        eos->setPressure(&tmpU, &tmpRho, &tmpU);
        P[0] = tmpU;

        // Step 2: integrate outward
        for (int i = 1; i < nz; ++i) {
            m[i] = m[i-1] + dm;

            

            // Hydrostatic
            double dPdm = -constants.G() * m[i-1] / (4.0 * M_PI * std::pow(r[i-1], 4));
            P[i] = P[i-1] + dPdm * dm;

            u[i] = u[0];
            tmpU = u[i];

            // Newton-Raphson solve for rho[i]
            double rho_i = rho[i-1];
            for (int iter = 0; iter < 20; ++iter) {
                tmpRho = rho_i;
                eos->setPressure(&tmpU, &tmpRho, &tmpU);
                double P_rho = tmpU;
                double f = P_rho - P[i];

                if (std::abs(f / P[i]) < tol) break;

                tmpRho = rho_i + 1e-6 * rho_i;
                eos->setPressure(&tmpU, &tmpRho, &tmpU);
                double P_plus = tmpU;
                double df = (P_plus - P_rho) / (1e-6 * rho_i);
                rho_i -= f / df;

                rho_i = std::max(rho_i, 1e-12);
            }

            rho[i] = rho_i;

            // Integrate radius
            double drdm = 1.0 / (4.0 * M_PI * std::pow(r[i-1], 2) * rho[i-1]);
            r[i] = r[i-1] + drdm * dm;
            T[i] = T[0];
        }

        ScalarField* frho = nodeList->getField<double>("density");
        ScalarField* fu   = nodeList->getField<double>("specificInternalEnergy");
        ScalarField* fP   = nodeList->getField<double>("pressure");
        ScalarField* fT   = nodeList->getField<double>("temperature");
        ScalarField* fr   = nodeList->getField<double>("mass");
        ScalarField* fm   = nodeList->getField<double>("radius");

        for (int i = 0; i < nz; ++i) {
            std::cout << m[i] << " " << rho[i] << std::endl;
            
            frho->setValue(i, rho[i]);
            fu->setValue(i, u[i]);
            fP->setValue(i, P[i]);
            fT->setValue(i, T[i]);
            fr->setValue(i, r[i]);
            fm->setValue(i, m[i]);
        }

        std::cout << "Initialized hydrostatic model with central T = " << centralTemperature << std::endl;
    }
};
