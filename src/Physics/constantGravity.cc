#include "physics.hh"

namespace Physics {
template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
public:
    ConstantGravity() {}

    ConstantGravity(DataBase* dataBase, PhysicalConstants& constants, Lin::Vector<dim>& gravityVector) : 
        Physics<dim>(dataBase,constants),
        gravityVector(gravityVector) {
        for (auto nodeList : dataBase->nodeLists) {
            int numNodes = nodeList->size();
            if (nodeList->getField("acceleration") == nullptr)
                nodeList->insertField<Lin::Vector<dim>>("acceleration");
            for (int i=0; i<numNodes; ++i)
                nodeList->getField("acceleration")[i] = gravityVector;
        }
    }

    virtual ~ConstantGravity() {}

    virtual void
    UpdateDerivatives(const double dt) {
        // compute accelerations
    }
};

using ConstantGravity1D = ConstantGravity<1>;
using ConstantGravity2D = ConstantGravity<2>;
using ConstantGravity3D = ConstantGravity<3>;

}
