#ifndef EQUATIONOFSTATEBASE_HH
#define EQUATIONOFSTATEBASE_HH

#include <vector>
#include "../Type/physicalConstants.hh"

// Base class for Equation of State
class EquationOfState {
public:
    EquationOfState(PhysicalConstants& constants) {}
    
    virtual ~EquationOfState() {}

    virtual double computePressure(double density, double internalEnergy) const = 0;

    virtual double computeInternalEnergy(double density, double pressure) const = 0;

    virtual double computeSoundSpeed(double density, double pressure) const = 0;
};

#endif // EQUATIONOFSTATEBASE_HH