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

    Field<Lin::Vector<dim>> 
    derivative(double t, const Field<Lin::Vector<dim>>& y, std::string yp) {
        // Example differential equation: dy/dt = y'
        // in this case, i don't use the field "y" to calculate y'
        Field<Lin::Vector<dim>> dydt("dydt-"+yp);
        NodeList* nodeList = this->nodeList;
        for (size_t i = 0; i < y.size(); ++i) {
            dydt.addValue(nodeList->getField<Lin::Vector<dim>>(yp)[i]);
        }
    return dydt;
}
};
