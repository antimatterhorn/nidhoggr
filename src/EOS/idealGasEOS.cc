#include "equationOfStateBase.hh"
#include <cmath>

class IdealGasEOS : public EquationOfState {
private:
    double gamma; 

public:
    IdealGasEOS(double specificHeatRatio) : gamma(specificHeatRatio) {}

    // Method to compute pressure given density and internal energy
    virtual double computePressure(double density, double internalEnergy) const override {
        return (gamma - 1.0) * density * internalEnergy;
    }

    // Method to compute internal energy given density and pressure
    virtual double computeInternalEnergy(double density, double pressure) const override {
        return pressure / ((gamma - 1.0) * density);
    }

    // Method to compute sound speed given density and pressure
    virtual double computeSoundSpeed(double density, double pressure) const override {
        return std::sqrt(gamma * pressure / density);
    }

    double getGamma() const { return gamma; }
};