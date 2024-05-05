#include "equationOfState.hh"
#include <cmath>

class IdealGasEOS : public EquationOfState {
private:
    double gamma; 

public:
    IdealGasEOS(double specificHeatRatio, PhysicalConstants& constants) : 
        EquationOfState(constants), 
        gamma(specificHeatRatio) {}

    // Method to compute pressure given density and internal energy
    virtual void 
    setPressure(Field<double>& pressure, const Field<double>& density, const Field<double>& internalEnergy) const override {
        //return (gamma - 1.0) * density * internalEnergy;
    }

    // Method to compute internal energy given density and pressure
    virtual void 
    setInternalEnergy(Field<double>& internalEnergy, const Field<double>& density, const Field<double>& pressure) const override {
        //return pressure / ((gamma - 1.0) * density);
    }

    // Method to compute sound speed given density and pressure
    virtual void 
    setSoundSpeed(Field<double>& soundSpeed, const Field<double>& density, const Field<double>& internalEnergy) const override {
        //return std::sqrt(gamma * pressure / density);
    }

    double 
    getGamma() const { return gamma; }
};