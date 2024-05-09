#include "physics.hh"
#include <iostream>

template <int dim>
class PointSourceGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> pointSourceLocation;
    double pointSourceMass;
public:
    PointSourceGravity() {}

    PointSourceGravity(NodeList* nodeList, PhysicalConstants& constants, Lin::Vector<dim>& pointSourceLocation, double pointSourceMass) : 
        Physics<dim>(nodeList,constants),
        pointSourceLocation(pointSourceLocation),
        pointSourceMass(pointSourceMass) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("acceleration");
        
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        this->derivFields.push_back(position);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        this->derivFields.push_back(velocity);
    }

    ~PointSourceGravity() {}

    virtual void
    EvaluateDerivatives(const Field<Lin::Vector<dim>>* initialState, Field<Lin::Vector<dim>>& deriv, const double t) override {
        // compute accelerations
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* acceleration = nodeList->getField<Lin::Vector<dim>>("acceleration");
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        for (int i=0; i<numNodes ; ++i) {
            Lin::Vector<dim> pos = position->getValue(i);
            double r = (pointSourceLocation - pos).magnitude();
            Lin::Vector<dim> rDir = (pointSourceLocation - pos)*(1.0/r);
            acceleration->setValue(i,pointSourceMass*constants.G()/(r*r)*rDir);
        }
        if(initialState->getNameString() == "position") {
            Field<Lin::Vector<dim>> *velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
            Field<Lin::Vector<dim>> dxdt;
            dxdt = *velocity + (*acceleration)*t;
            deriv.copyValues(dxdt);
        }
        else if(initialState->getNameString() == "velocity") {
            deriv.copyValues(acceleration);
        }
        
    }
};
