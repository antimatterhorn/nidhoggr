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

    virtual void setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const = 0;

    virtual void setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const = 0;

    virtual void setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const = 0;
};

#endif // EQUATIONOFSTATE_HH