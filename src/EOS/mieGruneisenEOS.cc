#include "equationOfState.hh"
#include <cmath>
#include <string>

class MieGruneisenEOS : public EquationOfState {
private:
    double rho0;     // Reference density
    double C0;       // Reference bulk sound speed
    double Gamma0;   // Constant Grüneisen parameter
    double S;        // Linear Hugoniot slope coefficient

public:
    MieGruneisenEOS(double rho0, double C0, double Gamma0, double S, PhysicalConstants& constants)
        : EquationOfState(constants), rho0(rho0), C0(C0), Gamma0(Gamma0), S(S) {}

    virtual ~MieGruneisenEOS() override {}

    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < density->size(); ++i) {
            double rho = density->getValue(i);
            double e   = internalEnergy->getValue(i);
            double eta = (rho / rho0) - 1.0;

            double denom = 1.0 - S * eta;
            double Pref = (rho0 * C0 * C0 * eta * (1.0 + 0.5 * (1.0 - S) * eta)) / (denom * denom);

            double P = Pref + Gamma0 * rho * e;
            pressure->setValue(i, P);
        }
    }

    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const override {
        for (int i = 0; i < density->size(); ++i) {
            double rho = density->getValue(i);
            double P   = pressure->getValue(i);
            double eta = (rho / rho0) - 1.0;

            double denom = 1.0 - S * eta;
            double Pref = (rho0 * C0 * C0 * eta * (1.0 + 0.5 * (1.0 - S) * eta)) / (denom * denom);

            double e = (P - Pref) / (Gamma0 * rho);
            internalEnergy->setValue(i, e);
        }
    }

    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < density->size(); ++i) {
            double rho = density->getValue(i);
            double e   = internalEnergy->getValue(i);
            double eta = (rho / rho0) - 1.0;

            double denom = 1.0 - S * eta;
            double Pref = (rho0 * C0 * C0 * eta * (1.0 + 0.5 * (1.0 - S) * eta)) / (denom * denom);
            double dPref_drho = (C0 * C0 / rho0) * (1 + eta) * (1 + 0.5 * (1 - S) * eta) / (denom * denom * denom);

            double cs2 = dPref_drho + Gamma0 * e;
            soundSpeed->setValue(i, std::sqrt(std::max(cs2, 0.0)));
        }
    }

    virtual void 
    setTemperature(Field<double>* temperature, Field<double>* density, Field<double>* internalEnergy) const override {
        throw std::runtime_error("Mie-GrüneisenEOS does not define temperature.");
    }

    virtual void 
    setInternalEnergyFromTemperature(Field<double>* internalEnergy, Field<double>* density, Field<double>* temperature) const override {
        throw std::runtime_error("Mie-GrüneisenEOS does not define internal energy from temperature.");
    }

    // Scalar pointer-based methods
    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const override {
        const double rho = *density;
        const double e   = *internalEnergy;
        const double eta = (rho / rho0) - 1.0;
        const double denom = 1.0 - S * eta;
        const double Pref = (rho0 * C0 * C0 * eta * (1.0 + 0.5 * (1.0 - S) * eta)) / (denom * denom);
        *pressure = Pref + Gamma0 * rho * e;
    }

    virtual void 
    setInternalEnergy(double* internalEnergy, double* density, double* pressure) const override {
        const double rho = *density;
        const double P   = *pressure;
        const double eta = (rho / rho0) - 1.0;
        const double denom = 1.0 - S * eta;
        const double Pref = (rho0 * C0 * C0 * eta * (1.0 + 0.5 * (1.0 - S) * eta)) / (denom * denom);
        *internalEnergy = (P - Pref) / (Gamma0 * rho);
    }

    virtual void 
    setSoundSpeed(double* soundSpeed, double* density, double* internalEnergy) const override {
        const double rho = *density;
        const double e   = *internalEnergy;
        const double eta = (rho / rho0) - 1.0;
        const double denom = 1.0 - S * eta;
        const double dPref_drho = (C0 * C0 / rho0) * (1 + eta) * (1 + 0.5 * (1 - S) * eta) / (denom * denom * denom);
        const double cs2 = dPref_drho + Gamma0 * e;
        *soundSpeed = std::sqrt(std::max(cs2, 0.0));
    }

    // Temperature is not defined for Mie-Grüneisen unless an explicit relation is known
    virtual void 
    setTemperature(double* temperature, double* density, double* internalEnergy) const override {
        // Placeholder: temperature undefined for general Mie-Grüneisen
        throw std::runtime_error("Mie-GrüneisenEOS does not define temperature.");
    }

    virtual void 
    setInternalEnergyFromTemperature(double* internalEnergy, double* density, double* temperature) const override {
        // Placeholder: no T-e relation defined
        throw std::runtime_error("Mie-GrüneisenEOS does not define internal energy from temperature.");
    }

    virtual std::string 
    name() const override {
        return "Mie-GrüneisenEOS";
    }
};
