#include "physics.hh"
#include "../Mesh/stellarGrid1d.hh"
#include "../EOS/equationOfState.hh"
#include <vector>

class StellarModel {
public:
    using Vector       = Lin::Vector<1>;
    using VectorField  = Field<Vector>;
    using ScalarField  = Field<double>;

    static void BuildHydrostaticModel(Mesh::StellarGrid1d& grid,
                                         NodeList& nodeList,
                                         double totalMass,
                                         double centralTemperature,
                                         EquationOfState* eos,
                                         PhysicalConstants& constants) {
        const int nz = grid.nz;
        const double dm = totalMass / nz;

        std::vector<double> m(nz), r(nz), rho(nz), u(nz), P(nz), T(nz);
        m[0] = 0.5 * dm;  // midpoint of first zone
        r[0] = 1e-5;      // small seed radius

        // Step 1: set central T, compute u, eos → P and rho
        T[0] = centralTemperature;
        Field<double> tmpU("tmpU", 1);
        Field<double> tmpT("tmpT", 1);
        Field<double> tmpRho("tmpRho", 1);

        tmpT.setValue(0, T[0]);
        tmpRho.setValue(0, 10.0);  // crude starting density

        eos->setInternalEnergyFromTemperature(&tmpU, &tmpRho, &tmpT);
        u[0] = tmpU[0];

        // Iteratively solve for rho such that P = EOS(ρ,u)
        double guessRho = tmpRho[0];
        for (int attempt = 0; attempt < 20; ++attempt) {
            tmpRho.setValue(0, guessRho);
            eos->setPressure(&tmpU, &tmpRho, &tmpU);
            eos->setPressure(&tmpU, &tmpRho, &tmpU);  // redundant, but safe
            P[0] = tmpU[0];

            // Simple convergence check — more sophisticated root finding possible
            double P_expected = P[0];
            eos->setInternalEnergyFromTemperature(&tmpU, &tmpRho, &tmpT);
            eos->setPressure(&tmpU, &tmpRho, &tmpU);
            double P_new = tmpU[0];

            if (std::abs(P_new - P_expected) / P_expected < 1e-6)
                break;

            guessRho *= 1.02;
        }
        rho[0] = guessRho;

        // Step 2: integrate outward
        for (int i = 1; i < nz; ++i) {
            m[i] = m[i-1] + dm;

            // Hydrostatic equilibrium
            double dPdm = -constants.G() * m[i-1] / (4.0 * M_PI * std::pow(r[i-1], 4));
            P[i] = P[i-1] + dPdm * dm;

            // Assume u is constant for now (isentropic)
            u[i] = u[0];
            tmpU.setValue(0, u[i]);

            // Solve EOS for rho
            double trialRho = rho[i-1];
            for (int iter = 0; iter < 10; ++iter) {
                tmpRho.setValue(0, trialRho);
                eos->setPressure(&tmpU, &tmpRho, &tmpU);
                double P_rho = tmpU[0];
                double relErr = (P_rho - P[i]) / P[i];
                if (std::abs(relErr) < 1e-6)
                    break;
                trialRho *= std::pow(P[i] / P_rho, 1.0 / eos->getGamma());
            }
            rho[i] = trialRho;

            // Integrate radius
            double drdm = 1.0 / (4.0 * M_PI * std::pow(r[i-1], 2) * rho[i-1]);
            r[i] = r[i-1] + drdm * dm;

            T[i] = T[0];  // constant T for now (or recompute later)
        }

        // Assign values to grid
        grid.m = m;
        grid.r = r;

        // Assign to NodeList fields
        ScalarField* frho = nodeList.getField<double>("density");
        ScalarField* fu   = nodeList.getField<double>("specificInternalEnergy");
        ScalarField* fP   = nodeList.getField<double>("pressure");
        ScalarField* fT   = nodeList.getField<double>("temperature");

        for (int i = 0; i < nz; ++i) {
            frho->setValue(i, rho[i]);
            fu->setValue(i, u[i]);
            fP->setValue(i, P[i]);
            fT->setValue(i, T[i]);
        }

        std::cout << "Hydrostatic model initialized with central T = " << centralTemperature << "\n";
    }
};