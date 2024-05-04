#include "physics.hh"

namespace Physics {
template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
public:

    ConstantGravity() {}

    ConstantGravity(NodeList* nodeListPtr, PhysicalConstants& constants, Lin::Vector<dim>& gravityVector) : 
        Physics<dim>(nodeListPtr,constants),
        gravityVector(gravityVector) {

    }

    virtual ~ConstantGravity() {}
};
}
