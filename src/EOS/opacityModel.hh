#pragma once

#include <vector>
#include "../Type/physicalConstants.hh"
#include "../DataBase/field.hh"

class OpacityModel {
public:
    OpacityModel(PhysicalConstants& constants) {}
    virtual ~OpacityModel() {}

    virtual void setOpacity(Field<double>* opacity, Field<double>* density, Field<double>* temperature) const = 0;
    virtual void setConductivity(Field<double>* conductivity, Field<double>* density, Field<double>* temperature) const = 0;

    virtual void setOpacity(double* opacity, double* density, double* temperature) const = 0;
    virtual void setConductivity(double* conductivity,double* density, double* temperature) const = 0;

    virtual std::string 
    name() const = 0;
};
