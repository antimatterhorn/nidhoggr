#include "equationOfStateBase.hh"
#include "../Math/physicalConstants.hh"
#include <cmath>

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
    virtual double computePressure(double density, double internalEnergy) const override {
        return kappa*pow(density,gamma);
    }

    // Method to compute internal energy given density and pressure
    virtual double computeInternalEnergy(double density, double pressure) const override {
        return 0; // need some kind of error that indicates this is not supported
    }

    // Method to compute sound speed given density and pressure
    virtual double computeSoundSpeed(double density, double pressure) const override {
        return std::sqrt(gamma*kappa*pow(density,gamma - 1.0));
    }

    double getGamma() const { return gamma; }
    double getKappa() const { return kappa; }
    double getIndex() const { return index; }
};