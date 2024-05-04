#include "physics.hh"

namespace Physics {
template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
public:
    Field<Lin::Vector<dim>> acceleration;

    ConstantGravity() {}

    ConstantGravity(NodeList* nodeListPtr, PhysicalConstants& constants, Lin::Vector<dim>& gravityVector) : 
        Physics<dim>(nodeListPtr,constants),
        gravityVector(gravityVector) {
        int numNodes = this->nodeList->size();
        if (this->nodeList->getField("acceleration") == nullptr) {
            acceleration = new Field<Lin::Vector<dim>>("acceleration", numNodes);
            this->nodeList->addField(&acceleration);
        } else {
            acceleration = *this->nodeList->getField("acceleration"); 
        }

        for (int i=0; i<numNodes; ++i) {
            acceleration[i] = gravityVector;
        }
    }

    virtual ~ConstantGravity() {}

    virtual void
    UpdateDerivatives(const double dt) {
        // compute accelerations
    }
};
}
