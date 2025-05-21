#include "physics.hh"
#include "../EOS/equationOfState.hh"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

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
        const double tol = 1e-6;
        const int maxIter = 20;
        const double dr = 1e-3;  // fixed radial step

        // Storage for final converged profile
        std::vector<double> best_m(nz), best_r(nz), best_rho(nz), best_u(nz), best_P(nz), best_T(nz);

        // Initial guesses for central density using secant method
        double rho_c0 = 1e5;
        double rho_c1 = 2e5;
        double M0 = 0.0, M1 = 0.0;

        for (int outer = 0; outer < maxIter; ++outer) {
            double rho_c = (outer == 0 ? rho_c0 : rho_c1);
            std::vector<double> m(nz), r(nz), rho(nz), u(nz), P(nz), T(nz);
            r[0] = 1e-5;
            m[0] = 0.5*dm;

            T[0] = centralTemperature;
            rho[0] = rho_c;

            eos->setInternalEnergyFromTemperature(&u[0], &rho[0], &T[0]);
            eos->setPressure(&P[0], &rho[0], &u[0]);

            double tmpU, tmpRho, tmpP;
            double alpha = 0.8;

            for (int i = 1; i < nz; ++i) {
                r[i] = r[i - 1] + dr;

                // Hydrostatic equilibrium
                double dPdr = -constants.G() * m[i - 1] * rho[i - 1] / (r[i - 1] * r[i - 1]);
                P[i] = P[i - 1] + dPdr * dr;

                
                T[i] = T[0] * std::pow(1.0 - double(i)/nz, alpha);
                eos->setInternalEnergyFromTemperature(&u[i], &rho[i], &T[i]);
                tmpU = u[i];

                double rho_i = rho[i - 1];
                for (int iter = 0; iter < 20; ++iter) {
                    tmpRho = rho_i;
                    eos->setPressure(&tmpP, &tmpRho, &tmpU);
                    double f = tmpP - P[i];
                    if (std::abs(f / P[i]) < tol) break;

                    double drho = 1e-6 * rho_i;
                    tmpRho = rho_i + drho;
                    eos->setPressure(&tmpP, &tmpRho, &tmpU);
                    double df = (tmpP - (tmpP - f)) / drho;
                    rho_i -= f / df;
                    rho_i = std::max(rho_i, 1e-12);
                }
                rho[i] = rho_i;

                double dm_i = 4.0 * M_PI * std::pow(r[i - 1], 2) * rho[i - 1] * dr;
                m[i] = m[i - 1] + dm_i;
            }

            double M_calc = m[nz - 1];
            std::cout << "Iteration " << outer << ": rho_c = " << rho_c << ", M_calc = " << M_calc << std::endl;

            if (std::abs((M_calc - totalMass) / totalMass) < tol) {
                best_m = m; best_r = r; best_rho = rho; best_u = u; best_P = P; best_T = T;
                break;
            }

            if (outer == 0) {
                M0 = M_calc;
                rho_c0 = rho_c;
            } else {
                M1 = M_calc;
                double rho_new = rho_c1 - (M1 - totalMass) * (rho_c1 - rho_c0) / (M1 - M0 + 1e-12);
                rho_c0 = rho_c1;
                M0 = M1;
                rho_c1 = std::max(rho_new, 1e-4);
            }
        }

        // Assign to fields
        ScalarField* frho = nodeList->getField<double>("density");
        ScalarField* fu   = nodeList->getField<double>("specificInternalEnergy");
        ScalarField* fP   = nodeList->getField<double>("pressure");
        ScalarField* fT   = nodeList->getField<double>("temperature");
        ScalarField* fr   = nodeList->getField<double>("radius");
        ScalarField* fm   = nodeList->getField<double>("mass");

        printTable(best_r,best_rho,best_T,best_P,best_u);

        for (int i = 0; i < nz; ++i) {
            frho->setValue(i, best_rho[i]);
            fu->setValue(i, best_u[i]);
            fP->setValue(i, best_P[i]);
            fT->setValue(i, best_T[i]);
            fr->setValue(i, best_r[i]);
            fm->setValue(i, best_m[i]);
        }
    }

    void printTable(const std::vector<double>& r,
                    const std::vector<double>& rho,
                    const std::vector<double>& T,
                    const std::vector<double>& P,
                    const std::vector<double>& u) {
        std::cout << std::fixed << std::setprecision(6);

        std::cout << std::setw(12) << "r"
                << std::setw(16) << "rho"
                << std::setw(16) << "T"
                << std::setw(16) << "P" 
                << std::setw(16) << "u" << "\n";

        for (size_t i = 0; i < r.size(); ++i) {
            std::cout << std::setw(12) << std::scientific << r[i]
                    << std::setw(16) << std::scientific << rho[i]
                    << std::setw(16) << std::scientific << T[i]
                    << std::setw(16) << std::scientific << P[i]
                    << std::setw(16) << std::scientific << u[i]
                    << "\n";
        }
    }
};


