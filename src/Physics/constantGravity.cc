#include "physics.hh"

template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
public:
    ConstantGravity() {}

    ConstantGravity(NodeList* _nodeList, PhysicalConstants& _constants, Lin::Vector<dim>& _gravityVector) : 
        Physics<dim>(_nodeList,_constants),
        gravityVector(_gravityVector) {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("acceleration");
        for (int i=0; i<numNodes; ++i)
            nodeList->getField<Lin::Vector<dim>>("acceleration")->setValue(i,gravityVector);
    }

    ~ConstantGravity() {}

    void
    EvaluateDerivatives(const double dt) override {
        // compute accelerations
    }
};
