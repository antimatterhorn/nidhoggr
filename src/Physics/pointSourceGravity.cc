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
        State state = this->state;
        state.enrollField<Lin::Vector<dim>>(position);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        state.enrollField<Lin::Vector<dim>>(velocity);

        state.insertDeriv<Lin::Vector<dim>>("d_position");
        state.insertDeriv<Lin::Vector<dim>>("d_velocity");
    }

    ~PointSourceGravity() {}

    virtual void
    EvaluateDerivatives(const State* initialState, State& deriv, const double t) override {
        //compute accelerations
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* position       = initialState->getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* acceleration   = nodeList->getField<Lin::Vector<dim>>("acceleration");
        Field<Lin::Vector<dim>>* velocity       = initialState->getField<Lin::Vector<dim>>("velocity");
 
        Field<Lin::Vector<dim>>* dxdt           = initialState->getDerivative<Lin::Vector<dim>>("d_position");
        Field<Lin::Vector<dim>>* dvdt           = initialState->getDerivative<Lin::Vector<dim>>("d_velocity");

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
            
        
        dxdt = *velocity + (*acceleration)*t;
        
        deriv.copyValues(dxdt);
        deriv.copyValues(acceleration);

    }
        // Method to estimate a suitable timestep based on the dynamics of the system
    virtual double 
    EstimateTimestep() const override {     
        double timestepCoefficient = 1e-4; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }
};
