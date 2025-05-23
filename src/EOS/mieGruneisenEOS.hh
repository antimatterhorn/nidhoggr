#pragma once

#include "equationOfState.hh"

class MieGruneisenEOS : public EquationOfState {
public:
    MieGruneisenEOS(double rho0, double C0, double Gamma0, double S, PhysicalConstants& constants);
    virtual ~MieGruneisenEOS() override;

    virtual void setPressure(Field<double>*, Field<double>*, Field<double>*) const override;
    virtual void setInternalEnergy(Field<double>*, Field<double>*, Field<double>*) const override;
    virtual void setSoundSpeed(Field<double>*, Field<double>*, Field<double>*) const override;
    virtual void setTemperature(Field<double>*, Field<double>*, Field<double>*) const override;
    virtual void setInternalEnergyFromTemperature(Field<double>*, Field<double>*, Field<double>*) const override;

    virtual void setPressure(double*, double*, double*) const override;
    virtual void setInternalEnergy(double*, double*, double*) const override;
    virtual void setSoundSpeed(double*, double*, double*) const override;
    virtual void setTemperature(double*, double*, double*) const override;
    virtual void setInternalEnergyFromTemperature(double*, double*, double*) const override;

    virtual std::string name() const override;

private:
    double rho0, C0, Gamma0, S;
};
