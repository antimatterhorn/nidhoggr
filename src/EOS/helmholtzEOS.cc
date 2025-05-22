#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include "equationOfState.hh"

class HelmholtzEOS : public EquationOfState {
private:
    std::vector<double> logRhoGrid;
    std::vector<double> logTGrid;

    std::vector<std::vector<double>> PTable;
    std::vector<std::vector<double>> UTable;
    std::vector<std::vector<double>> CsTable;

    const PhysicalConstants constants;

    size_t findIndex(const std::vector<double>& grid, double value) const;
    double bilinearInterp(const std::vector<std::vector<double>>& table,
                          double logRho, double logT) const;private:
public:
    HelmholtzEOS(const std::string& tableFile, PhysicalConstants& constants) : 
        EquationOfState(constants), constants(constants) {}

    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const override;
    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const override;
    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const override;
    virtual void 
    setTemperature(Field<double>* temperature, Field<double>* density, Field<double>* internalEnergy) const override;
    virtual void 
    setInternalEnergyFromTemperature(Field<double>* internalEnergy, Field<double>* density, Field<double>* temperature);

    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const override;
    virtual void 
    setInternalEnergy(double* internalEnergy, double* density, double* pressure) const override;
    virtual void 
    setSoundSpeed(double* soundSpeed, double* density,double* internalEnergy) const override;
    virtual void 
    setTemperature(double* temperature, double* density, double* internalEnergy) const override;
    virtual void 
    setInternalEnergyFromTemperature(double* internalEnergy, double* density, double* temperature) const override;

    virtual double 
    getGamma() const override { return 0; };

};