#include "physicsBase.hh"

namespace Physics {
template <int dim>
class ConstantGravity : public PhysicsBase<dim> {
protected:
    Lin::Vector<dim> gravityVector;
public:
    ConstantGravity() {}

    ConstantGravity(DataBase* dataBase, PhysicalConstants& constants, Lin::Vector<dim>& gravityVector) : 
        PhysicsBase<dim>(dataBase,constants),
        gravityVector(gravityVector) {
        for (auto nodeList : dataBase->nodeLists) {
            int numNodes = nodeList->size();
            if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
                nodeList->insertField<Lin::Vector<dim>>("acceleration");
            for (int i=0; i<numNodes; ++i)
                nodeList->getField<Lin::Vector<dim>>("acceleration")->setValue(i,gravityVector);
        }
    }

    virtual ~ConstantGravity() {}

    virtual void
    UpdateDerivatives(const double dt) {
        // compute accelerations
    }
};
}
