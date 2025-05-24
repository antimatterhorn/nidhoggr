#include "equationOfState.hh"
#include <cmath>
#include <stdexcept>
#include <string>
#include <algorithm>

class TillotsonEOS : public EquationOfState {
private:
    double rho0, A, B, alpha, beta, a, b, e0, eiv, ecv;

    inline double computeEta(double rho) const { return rho / rho0; }
    inline double computeMu(double eta) const { return eta - 1.0; }

    double computePressure(double rho, double e) const {
        double eta = computeEta(rho);
        double mu = computeMu(eta);

        if (rho >= rho0 || e < eiv) {
            // Compressed or low-energy regime
            return (a + b / (1.0 + e / (e0 * eta * eta))) * rho * e + A * mu + B * mu * mu;
        } else if (e > ecv) {
            // Fully vaporized
            return a * rho * e + (b * rho * e) / (1.0 + e / (e0 * eta * eta));
        } else {
            // Mixed phase
            double z = (rho0 / rho) - 1.0;
            double exp_beta = std::exp(-beta * z);
            double exp_alpha = std::exp(-alpha * z * z);
            return (a * rho * e +
                    ((b * rho * e) / (1.0 + e / (e0 * eta * eta)) + A * mu * exp_beta) * exp_alpha);
        }
    }

public:
    TillotsonEOS(double rho0_, double A_, double B_, double alpha_, double beta_,
                 double a_, double b_, double e0_, double eiv_, double ecv_,
                 PhysicalConstants& constants)
        : EquationOfState(constants),
        rho0(constants.convertDensity(rho0_)),
        A(constants.convertPressure(A_)),
        B(constants.convertPressure(B_)),
        alpha(alpha_),
        beta(beta_),
        a(a_),
        b(b_),
        e0(constants.convertSpecificEnergy(e0_)),
        eiv(constants.convertSpecificEnergy(eiv_)),
        ecv(constants.convertSpecificEnergy(ecv_)) {}

    virtual ~TillotsonEOS() override {}

    // Field-based methods
    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < density->size(); ++i) {
            (*pressure)[i] = computePressure((*density)[i], (*internalEnergy)[i]);
        }
    }

    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const override {
        throw std::runtime_error("TillotsonEOS does not support inversion to internal energy.");
    }

    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < density->size(); ++i) {
            const double rho = (*density)[i];
            const double e   = (*internalEnergy)[i];

            const double drho = 1e-4 * rho;
            const double de = 1e-4 * e;

            double P0 = computePressure(rho, e);
            double Prho = computePressure(rho + drho, e);
            double Pe = computePressure(rho, e + de);

            double dPdrho = (Prho - P0) / drho;
            double dPde = (Pe - P0) / de;

            double cs2 = dPdrho + dPde * e / rho;
            (*soundSpeed)[i] = std::sqrt(std::max(cs2, 0.0));
        }
    }

    virtual void 
    setTemperature(Field<double>*, Field<double>*, Field<double>*) const override {
        throw std::runtime_error("TillotsonEOS does not define temperature.");
    }

    virtual void 
    setInternalEnergyFromTemperature(Field<double>*, Field<double>*, Field<double>*) const override {
        throw std::runtime_error("TillotsonEOS does not define internal energy from temperature.");
    }

    // Scalar-based methods
    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const override {
        *pressure = computePressure(*density, *internalEnergy);
    }

    virtual void 
    setInternalEnergy(double*, double*, double*) const override {
        throw std::runtime_error("TillotsonEOS does not support inversion to internal energy.");
    }

    virtual void 
    setSoundSpeed(double* soundSpeed, double* density, double* internalEnergy) const override {
        const double rho = *density;
        const double e = *internalEnergy;

        const double drho = 1e-4 * rho;
        const double de = 1e-4 * e;

        double P0 = computePressure(rho, e);
        double Prho = computePressure(rho + drho, e);
        double Pe = computePressure(rho, e + de);

        double dPdrho = (Prho - P0) / drho;
        double dPde = (Pe - P0) / de;

        double cs2 = dPdrho + dPde * e / rho;
        *soundSpeed = std::sqrt(std::max(cs2, 0.0));
    }

    virtual void 
    setTemperature(double*, double*, double*) const override {
        throw std::runtime_error("TillotsonEOS does not define temperature.");
    }

    virtual void 
    setInternalEnergyFromTemperature(double*, double*, double*) const override {
        throw std::runtime_error("TillotsonEOS does not define internal energy from temperature.");
    }

    virtual std::string 
    name() const override {
        return "TillotsonEOS";
    }
};
