#include "equationOfState.hh"
#include <cmath>
#include <stdexcept>
#include <string>

class IsothermalEOS : public EquationOfState {
private:
    double cs;  // constant sound speed

public:
    IsothermalEOS(double soundSpeed, PhysicalConstants& constants)
        : EquationOfState(constants),
          cs(soundSpeed) {}

    // --- Field versions ---
    virtual void setPressure(Field<double>* pressure, Field<double>* density, Field<double>*) const override {
        for (int i = 0; i < pressure->size(); ++i) {
            pressure->setValue(i, (*density)[i] * cs * cs);
        }
    }

    virtual void setInternalEnergy(Field<double>* internalEnergy, Field<double>*, Field<double>*) const override {
        throw std::runtime_error("IsothermalEOS does not define internal energy.");
    }

    virtual void setSoundSpeed(Field<double>* soundSpeed, Field<double>*, Field<double>*) const override {
        for (int i = 0; i < soundSpeed->size(); ++i) {
            soundSpeed->setValue(i, cs);
        }
    }

    virtual void setTemperature(Field<double>*, Field<double>*, Field<double>*) const override {
        throw std::runtime_error("IsothermalEOS does not define temperature.");
    }

    virtual void setInternalEnergyFromTemperature(Field<double>*, Field<double>*, Field<double>*) const override {
        throw std::runtime_error("IsothermalEOS does not define internal energy from temperature.");
    }

    // --- Scalar versions ---
    virtual void setPressure(double* pressure, double* density, double*) const override {
        *pressure = (*density) * cs * cs;
    }

    virtual void setInternalEnergy(double*, double*, double*) const override {
        throw std::runtime_error("IsothermalEOS does not define internal energy.");
    }

    virtual void setSoundSpeed(double* soundSpeed, double*, double*) const override {
        *soundSpeed = cs;
    }

    virtual void setTemperature(double*, double*, double*) const override {
        throw std::runtime_error("IsothermalEOS does not define temperature.");
    }

    virtual void setInternalEnergyFromTemperature(double*, double*, double*) const override {
        throw std::runtime_error("IsothermalEOS does not define internal energy from temperature.");
    }

    // --- Accessors ---
    virtual std::string name() const override {
        return "IsothermalEOS";
    }
};
