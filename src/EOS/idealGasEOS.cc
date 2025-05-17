#include "equationOfState.hh"
#include <cmath>
#include <iostream>

class IdealGasEOS : public EquationOfState {
private:
    double gamma; 

public:
    IdealGasEOS(double specificHeatRatio, PhysicalConstants& constants) : 
        EquationOfState(constants), 
        gamma(specificHeatRatio) {}

    // Method to compute pressure given density and internal energy
    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const override {   
        for(int i=0;i<pressure->size();++i)
            pressure->setValue(i,(gamma - 1.0) * density->getValue(i) * internalEnergy->getValue(i));
    }

    // Method to compute internal energy given density and pressure
    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const override {
        for (int i = 0; i < internalEnergy->size(); ++i) {
            const double rho = density->getValue(i);
            const double P = pressure->getValue(i);
            internalEnergy->setValue(i, P / ((gamma - 1.0) * rho));
        }
    }

    // Method to compute sound speed given density and pressure
    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const override {
        for(int i=0;i<soundSpeed->size();++i)
            soundSpeed->setValue(i,std::sqrt(gamma * (gamma - 1.0) * density->getValue(i) * internalEnergy->getValue(i) / density->getValue(i)));
    }

    double 
    getGamma() const { return gamma; }
};