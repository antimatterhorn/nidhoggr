#include "physics.hh"
#include "../EOS/equationOfState.hh"
#include "../Utilities/printTable.hh"
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
    double totalMass, centralTemperature, radius;

    StellarEvolution(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, 
        double totalMass, double radius, double centralTemperature)
        : Physics<1>(nodeList, constants), numZones(nodeList->size()), eos(eos),
        totalMass(totalMass), radius(radius), centralTemperature(centralTemperature) {

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
        const int maxIter = 30;

        // Fixed spacing in radius
        const double dr = radius / nz;

        // Initial guess range for central density
        double rho_c0 = 1e4, rho_c1 = 2e4;
        double M0 = 0.0, M1 = 0.0;

        std::vector<double> best_rho, best_T, best_u, best_P, best_m, best_r;

        for (int outer = 0; outer < maxIter; ++outer) {
            double rho_c = (outer == 0 ? rho_c0 : rho_c1);

            // Init arrays
            std::vector<double> rho(nz), T(nz), u(nz), P(nz), m(nz), r(nz);
            r[0] = 1e-5;
            m[0] = 0.0;
            rho[0] = rho_c;
            T[0] = centralTemperature;
            double gamma = eos->getGamma();
            eos->setInternalEnergyFromTemperature(&u[0], &rho[0], &T[0]);
            eos->setPressure(&P[0], &rho[0], &u[0]);

            for (int i = 1; i < nz; ++i) {
                r[i] = r[i-1] + dr;

                // Integrate mass
                double dm_dr = 4 * M_PI * r[i-1] * r[i-1] * rho[i-1];
                m[i] = m[i-1] + dm_dr * dr;

                // Hydrostatic equilibrium: dP/dr = -G m rho / r^2
                double dP_dr = -constants.G() * m[i-1] * rho[i-1] / (r[i-1] * r[i-1]);
                P[i] = P[i-1] + dP_dr * dr;

                //T[i] = T[i-1]/P[i-1]*dP_dr*dr*(gamma-1)/gamma + T[i-1];
                T[i] = T[0]; // change this
                eos->setInternalEnergyFromTemperature(&u[i], &rho[i-1], &T[i]);

                // Solve for rho_i such that P(rho_i, T[i]) = P[i]
                double rho_i = rho[i-1];
                for (int iter = 0; iter < 20; ++iter) {
                    double tmp_u, tmp_P;
                    eos->setInternalEnergyFromTemperature(&tmp_u, &rho_i, &T[i]);
                    eos->setPressure(&tmp_P, &rho_i, &tmp_u);
                    double f = tmp_P - P[i];
                    if (std::abs(f / P[i]) < tol) break;

                    // Finite difference
                    double drho = 1e-6 * rho_i;
                    double rho_pert = rho_i + drho;
                    double tmp_u_plus, tmp_P_plus;
                    eos->setInternalEnergyFromTemperature(&tmp_u_plus, &rho_pert, &T[i]);
                    eos->setPressure(&tmp_P_plus, &rho_pert, &tmp_u_plus);
                    double df = (tmp_P_plus - tmp_P) / drho;

                    rho_i -= f / df;
                    rho_i = std::max(rho_i, 1e-12);
                }

                rho[i] = rho_i;
            }

            double Mcalc = m[nz - 1];
            std::cout << "Iteration " << outer << ": rho_c = " << rho_c << ", M = " << Mcalc << "\n";

            if (std::abs((Mcalc - totalMass) / totalMass) < tol) {
                best_rho = rho;
                best_T = T;
                best_u = u;
                best_P = P;
                best_m = m;
                best_r = r;
                break;
            }

            // Update secant method
            if (outer == 0) {
                M0 = Mcalc;
                rho_c0 = rho_c;
            } else {
                M1 = Mcalc;
                double rho_new = rho_c1 - (M1 - totalMass) * (rho_c1 - rho_c0) / (M1 - M0 + 1e-12);
                rho_c0 = rho_c1;
                M0 = M1;
                rho_c1 = std::max(rho_new, 1e-4);
            }
        }

        // Rescale radius to hit target R
        double r_scale = radius / best_r.back();
        for (double& ri : best_r) ri *= r_scale;

        // Assign to fields
        ScalarField* frho = nodeList->getField<double>("density");
        ScalarField* fu   = nodeList->getField<double>("specificInternalEnergy");
        ScalarField* fP   = nodeList->getField<double>("pressure");
        ScalarField* fT   = nodeList->getField<double>("temperature");
        ScalarField* fr   = nodeList->getField<double>("radius");
        ScalarField* fm   = nodeList->getField<double>("mass");
        for (int i = 0; i < nz; ++i) {
            frho->setValue(i, best_rho[i]);
            fT->setValue(i, best_T[i]);
            fu->setValue(i, best_u[i]);
            fP->setValue(i, best_P[i]);
            fm->setValue(i, best_m[i]);
            fr->setValue(i, best_r[i]);
        }



        printTable(frho->size(), *fr, *frho, *fm, *fu, *fP, *fT);
    }
};


