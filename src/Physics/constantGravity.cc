#include "physicsBase.hh"

namespace Physics {
template <int dim>
class ConstantGravity : public PhysicsBase<dim> {
protected:
    Lin::Vector<dim> gravityVector;
public:
    ConstantGravity() {}

    ConstantGravity(NodeList* _nodeList, PhysicalConstants& _constants, Lin::Vector<dim>& _gravityVector) : 
        PhysicsBase<dim>(_nodeList,_constants),
        gravityVector(_gravityVector) {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("acceleration");
        for (int i=0; i<numNodes; ++i)
            nodeList->getField<Lin::Vector<dim>>("acceleration")->setValue(i,gravityVector);
    }

    virtual ~ConstantGravity() {}

    virtual void
    UpdateDerivatives(const double dt) override {
        // compute accelerations
    }
};
}
