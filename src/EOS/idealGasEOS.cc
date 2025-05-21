#include "equationOfState.hh"
#include <cmath>
#include <iostream>

class IdealGasEOS : public EquationOfState {
private:
    double gamma; 
    PhysicalConstants& constants;
public:
    IdealGasEOS(double specificHeatRatio, PhysicalConstants& constants) : 
        EquationOfState(constants), constants(constants), 
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
            soundSpeed->setValue(i,std::sqrt(gamma * (gamma - 1.0) * internalEnergy->getValue(i)));
    }

    virtual void 
    setTemperature(Field<double>* temperature, Field<double>* density, Field<double>* internalEnergy) const override {
        const double kB = constants.kB();
        const double mu = 1.0;  // mean molecular weight (placeholder)
        const double mH = constants.protonMass();

        for (int i = 0; i < temperature->size(); ++i) {
            double rho = density->getValue(i);
            double u = internalEnergy->getValue(i);
            double T = (gamma - 1.0) * u * mu * mH / kB;
            temperature->setValue(i, T);
        }
    }

    virtual void 
    setInternalEnergyFromTemperature(Field<double>* internalEnergy, Field<double>* density, Field<double>* temperature) const override {
        const double kB = constants.kB();
        const double mu = 1.0;
        const double mH = constants.protonMass();

        for (int i = 0; i < internalEnergy->size(); ++i) {
            double T = temperature->getValue(i);
            internalEnergy->setValue(i, kB * T / ((gamma - 1.0) * mu * mH));
        }
    }

    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const override {
        *pressure = (gamma - 1.0) * (*density) * (*internalEnergy);
    }

    virtual void 
    setInternalEnergy(double* internalEnergy, double* density, double* pressure) const override {
        *internalEnergy = (*pressure) / ((gamma - 1.0) * (*density));
    }

    virtual void 
    setSoundSpeed(double* soundSpeed, double* density,double* internalEnergy) const override {
        *soundSpeed = std::sqrt(gamma * (gamma - 1.0) * (*internalEnergy));
    }

    virtual void 
    setTemperature(double* temperature, double* density, double* internalEnergy) const override {
        const double kB = constants.kB();
        const double mu = 1.0;  // mean molecular weight (placeholder)
        const double mH = constants.protonMass();

        *temperature = (gamma - 1.0) * (*internalEnergy) * mu * mH / kB;
    }

    virtual void 
    setInternalEnergyFromTemperature(double* internalEnergy, double* density, double* temperature) const override {
        const double kB = constants.kB();
        const double mu = 1.0;
        const double mH = constants.protonMass();

        *internalEnergy = (*temperature) * kB / ((gamma - 1.0) * mu * mH);
    }


    double 
    getGamma() const { return gamma; }
};