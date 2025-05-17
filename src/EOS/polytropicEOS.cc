#include "equationOfState.hh"
#include <cmath>
#include <stdexcept>

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
            gamma = (index + 1.0)/index;
        }

    // Method to compute pressure given density and internal energy
    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < pressure->size(); ++i) {
            const double rho = density->getValue(i);
            pressure->setValue(i, kappa * std::pow(rho, gamma));
        }
    }

    // Method to compute internal energy given density and pressure
    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const override {
        throw std::runtime_error("PolytropicEOS does not support setting internal energy from pressure and density.");
    }

    // Method to compute sound speed given density and pressure
    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < soundSpeed->size(); ++i) {
            const double rho = density->getValue(i);
            soundSpeed->setValue(i, std::sqrt(gamma * kappa * std::pow(rho, gamma - 1.0)));
        }
    }

    double getGamma() const { return gamma; }
    double getKappa() const { return kappa; }
    double getIndex() const { return index; }
};