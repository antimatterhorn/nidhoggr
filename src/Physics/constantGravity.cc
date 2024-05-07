#include "physics.hh"
#include <iostream>

template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
    Field<Lin::Vector<dim>> dydt;
    Field<Lin::Vector<dim>> dvdt;
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
        
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        this->derivFields.push_back(position);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        this->derivFields.push_back(velocity);
    }

    ~ConstantGravity() {}

    virtual Field<Lin::Vector<dim>>*
    EvaluateDerivatives(const Field<Lin::Vector<dim>>& field, const double t) override {
        // compute accelerations
        std::cout << "in eval";
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        dydt = Field<Lin::Vector<dim>>("dydt",numNodes);
        if(field.getName() == "position") {
            Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
            std::cout << "in position";
            dydt.copyValues(velocity);
        }
        else if(field.getName() == "velocity") {
            Field<Lin::Vector<dim>>* acceleration = nodeList->getField<Lin::Vector<dim>>("acceleration");
            dydt.copyValues(acceleration);
        }
        return &dydt;
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
