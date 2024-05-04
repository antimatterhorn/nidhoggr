#ifndef HYDRO_HH
#define HYDRO_HH

#include "physics.hh"

namespace Physics {
template <int dim>
class Hydro : public Physics<dim> {
protected:
public:

    Hydro() {}

    Hydro(NodeList* nodeListPtr, PhysicalConstants& constants) : 
        Physics<dim>(nodeListPtr,constants) {

    }

    virtual ~Hydro() {}
};
}

#endif