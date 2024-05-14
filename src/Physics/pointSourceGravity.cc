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

    PointSourceGravity(NodeList* nodeList, 
                        PhysicalConstants& constants, 
                        Lin::Vector<dim>& pointSourceLocation, 
                        double pointSourceMass) : 
        Physics<dim>(nodeList,constants),
        pointSourceLocation(pointSourceLocation),
        pointSourceMass(pointSourceMass) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("acceleration");
        
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        State* state = &this->state;
        state->enrollField<Lin::Vector<dim>>(position);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        state->enrollField<Lin::Vector<dim>>(velocity);

        state->insertDeriv<Lin::Vector<dim>>("Dposition");
        state->insertDeriv<Lin::Vector<dim>>("Dvelocity");
    }

    ~PointSourceGravity() {}

    virtual void
    EvaluateDerivatives(const State* initialState, State* deriv, const double t) override {
        //compute accelerations
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* position       = initialState->getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* acceleration   = nodeList->getField<Lin::Vector<dim>>("acceleration");
        Field<Lin::Vector<dim>>* velocity       = initialState->getField<Lin::Vector<dim>>("velocity");

        NodeList* dd = deriv->getNodeList();
        Field<Lin::Vector<dim>>* dxdt = dd->getField<Lin::Vector<dim>>("Dposition");  
        Field<Lin::Vector<dim>>* dvdt = dd->getField<Lin::Vector<dim>>("Dvelocity");
        dtmin = 1e30;
        #pragma omp parllel for
        for (int i=0; i<numNodes ; ++i) {
            Lin::Vector<dim> pos = position->getValue(i);
            Lin::Vector<dim> r = (pointSourceLocation - pos);
            Lin::Vector<dim> a = pointSourceMass*constants.G()/(r.mag2())*r.normal();
            acceleration->setValue(i,a);
            double amag = a.mag2();
            double vmag = velocity->getValue(i).mag2();
            dtmin = std::min(dtmin,vmag/amag);
            dxdt->setValue(i,velocity->getValue(i)+t*(acceleration->getValue(i)));
            dvdt->setValue(i,acceleration->getValue(i));
        }
        
        // deriv.copyValues(dxdt);
        // deriv.copyValues(acceleration);

    }
        // Method to estimate a suitable timestep based on the dynamics of the system
    virtual double 
    EstimateTimestep() const override {     
        double timestepCoefficient = 1e-4; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }
};
