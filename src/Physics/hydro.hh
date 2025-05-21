#ifndef HYDRO_HH
#define HYDRO_HH

#include "physics.hh"
#include "../EOS/equationOfState.hh"

template <int dim>
class Hydro : public Physics<dim> {
protected:
    EquationOfState* eos;
public:

    Hydro(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos) : 
        Physics<dim>(nodeList,constants), eos(eos) {
        VerifyHydroFields(nodeList);
    }

    virtual ~Hydro() {}

    virtual void
    VerifyHydroFields(NodeList* nodeList) {
        for (const std::string& name : 
            {"pressure", "density", "specificInternalEnergy", "soundSpeed"}) {
            if (nodeList->getField<double>(name) == nullptr)
                nodeList->insertField<double>(name);
        }
    }

    virtual std::string name() const override { return "hydro"; }
    virtual std::string description() const override {
        return "Some kind of hydro"; }
};

#endif