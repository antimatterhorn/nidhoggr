#ifndef EQUATIONOFSTATE_HH
#define EQUATIONOFSTATE_HH

#include <vector>
#include "../Type/physicalConstants.hh"
#include "../DataBase/field.hh"

// Base class for Equation of State
class EquationOfState {
public:
    EquationOfState(PhysicalConstants& constants) {}
    
    virtual ~EquationOfState() {}

    // Field-based methods

    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const = 0;

    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const = 0;

    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const = 0;

    virtual void 
    setTemperature(Field<double>* temperature, Field<double>* density, Field<double>* internalEnergy) const = 0;

    virtual void 
    setInternalEnergyFromTemperature(Field<double>* internalEnergy, Field<double>* density, Field<double>* temperature) const = 0;

    // Scalar-based methods

    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const = 0;

    virtual void 
    setInternalEnergy(double* internalEnergy, double* density, double* pressure) const = 0;

    virtual void 
    setSoundSpeed(double* soundSpeed, double* density,double* internalEnergy) const = 0;

    virtual void 
    setTemperature(double* temperature, double* density, double* internalEnergy) const = 0;

    virtual void 
    setInternalEnergyFromTemperature(double* internalEnergy, double* density, double* temperature) const = 0;

    virtual double 
    getGamma() const = 0;
};

#endif // EQUATIONOFSTATE_HH