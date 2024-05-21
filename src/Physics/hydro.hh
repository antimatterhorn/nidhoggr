#ifndef HYDRO_HH
#define HYDRO_HH

#include "physics.hh"
#include "../EOS/equationOfState.hh"

template <int dim>
class Hydro : public Physics<dim> {
protected:
    EquationOfState* eos;
public:

    Hydro() {}

    Hydro(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos) : 
        Physics<dim>(nodeList,constants), eos(eos) {
        VerifyHydroFields(nodeList);
    }

    virtual ~Hydro() {}

    virtual void
    VerifyHydroFields(NodeList* nodeList) {

        if (nodeList->getField<double>("density") == nullptr)
            nodeList->insertField<double>("density");
        if (nodeList->getField<double>("pressure") == nullptr)
            nodeList->insertField<double>("pressure");
        if (nodeList->getField<double>("specificInternalEnergy") == nullptr)
            nodeList->insertField<double>("specificInternalEnergy");
        if (nodeList->getField<double>("soundSpeed") == nullptr)
            nodeList->insertField<double>("soundSpeed");
    }
};

#endif