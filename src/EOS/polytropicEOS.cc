#include "equationOfState.hh"
#include <cmath>
#include <stdexcept>
#include <iostream>

class PolytropicEOS : public EquationOfState {
private:
    double index;
    double gamma; 
    double kappa;

public:
    PolytropicEOS(double polytropicConstant, double polytropicIndex, PhysicalConstants& constants): 
        EquationOfState(constants), 
        kappa(polytropicConstant), 
        index(polytropicIndex) {
            gamma = (index + 1.0) / index;
        }

    // --- Field versions ---
    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < pressure->size(); ++i) {
            const double rho = density->getValue(i);
            pressure->setValue(i, kappa * std::pow(rho, gamma));
        }
    }

    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const override {
        throw std::runtime_error("PolytropicEOS does not support setting internal energy from pressure and density.");
    }

    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < soundSpeed->size(); ++i) {
            const double rho = density->getValue(i);
            soundSpeed->setValue(i, std::sqrt(gamma * kappa * std::pow(rho, gamma - 1.0)));
        }
    }

    virtual void 
    setTemperature(Field<double>* temperature, Field<double>* density, Field<double>* internalEnergy) const override {
        throw std::runtime_error("PolytropicEOS does not define temperature.");
    }

    virtual void 
    setInternalEnergyFromTemperature(Field<double>* internalEnergy, Field<double>* density, Field<double>* temperature) const override {
        throw std::runtime_error("PolytropicEOS does not define internal energy from temperature.");
    }

    // --- Scalar versions ---
    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const override {
        *pressure = kappa * std::pow(*density, gamma);
    }

    virtual void 
    setInternalEnergy(double* internalEnergy, double* density, double* pressure) const override {
        throw std::runtime_error("PolytropicEOS does not support setting internal energy from pressure and density.");
    }

    virtual void 
    setSoundSpeed(double* soundSpeed, double* density, double* internalEnergy) const override {
        *soundSpeed = std::sqrt(gamma * kappa * std::pow(*density, gamma - 1.0));
    }

    virtual void 
    setTemperature(double* temperature, double* density, double* internalEnergy) const override {
        throw std::runtime_error("PolytropicEOS does not define temperature.");
    }

    virtual void 
    setInternalEnergyFromTemperature(double* internalEnergy, double* density, double* temperature) const override {
        throw std::runtime_error("PolytropicEOS does not define internal energy from temperature.");
    }

    // --- Accessors ---
    double getGamma() const { return gamma; }
    double getKappa() const { return kappa; }
    double getIndex() const { return index; }
};
