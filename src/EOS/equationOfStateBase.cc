#include <vector>

// Base class for Equation of State
class EquationOfStateBase {
public:
    virtual ~EquationOfStateBase() {}

    virtual double computePressure(double density, double internalEnergy) const = 0;

    virtual double computeInternalEnergy(double density, double pressure) const = 0;

    virtual double computeSoundSpeed(double density, double pressure) const = 0;
};
