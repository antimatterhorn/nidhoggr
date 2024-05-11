#include "physics.hh"
#include <iostream>

template <int dim>
class PointSourceGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> pointSourceLocation;
    double pointSourceMass;
    double dtmin;
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

        Field<Lin::Vector<dim>>* acceleration   = nodeList->getField<Lin::Vector<dim>>("acceleration");
        Field<Lin::Vector<dim>>* position       = nodeList->getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* velocity       = nodeList->getField<Lin::Vector<dim>>("velocity");

        if(initialState->getNameString() == "position") {
            dtmin = 1e+30;
            #pragma omp parllel for
            for (int i=0; i<numNodes ; ++i) {
                Lin::Vector<dim> pos = position->getValue(i);
                Lin::Vector<dim> r = (pointSourceLocation - pos);
                Lin::Vector<dim> a = pointSourceMass*constants.G()/(r.mag2())*r.normal();
                acceleration->setValue(i,a);
                double amag = a.mag2();
                double vmag = velocity->getValue(i).mag2();
                dtmin = std::min(dtmin,vmag/amag);
            }
            
            Field<Lin::Vector<dim>> dxdt;
            dxdt = *velocity + (*acceleration)*t;
            deriv.copyValues(dxdt);
        }
        else if(initialState->getNameString() == "velocity") {
            deriv.copyValues(acceleration);
        }
        
    }
        // Method to estimate a suitable timestep based on the dynamics of the system
    virtual double 
    EstimateTimestep() const override {     
        double timestepCoefficient = 1e-4; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }
};
