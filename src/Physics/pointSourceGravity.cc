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
        // std::shared_ptr<Field<Lin::Vector<dim>>> positionSharedPtr(position);
        State<dim>* state = &this->state;        
        state->template addField<Lin::Vector<dim>>(position);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        //std::shared_ptr<Field<Lin::Vector<dim>>> velocitySharedPtr(velocity);
        state->template addField<Lin::Vector<dim>>(velocity);
    }

    ~PointSourceGravity() {}

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        //compute accelerations
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* position       = initialState->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* acceleration   = nodeList->getField<Lin::Vector<dim>>("acceleration");
        // ^ this field is just for reference and isn't actually used to calculate anything
        Field<Lin::Vector<dim>>* velocity       = initialState->template getField<Lin::Vector<dim>>("velocity");

        Field<Lin::Vector<dim>>* dxdt           = deriv.template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* dvdt           = deriv.template getField<Lin::Vector<dim>>("velocity");

        dtmin = 1e30;
        #pragma omp parllel for
        for (int i=0; i<numNodes ; ++i) {
            Lin::Vector<dim> pos = position->getValue(i);
            Lin::Vector<dim> r = (pointSourceLocation - pos);
            Lin::Vector<dim> a = pointSourceMass*constants.G()/(r.mag2())*r.normal();
            Lin::Vector<dim> v = velocity->getValue(i);
            acceleration->setValue(i,a);
            double amag = a.mag2();
            double vmag = v.mag2();
            dtmin = std::min(dtmin,vmag/amag);
            dxdt->setValue(i,v+dt*a);
            dvdt->setValue(i,a);
        }


    }

    virtual double 
    EstimateTimestep() const override {     
        double timestepCoefficient = 1e-4; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        State<dim> state = this->state;

        Field<Lin::Vector<dim>>* fposition       = finalState->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* fvelocity       = finalState->template getField<Lin::Vector<dim>>("velocity");

        Field<Lin::Vector<dim>>* sposition       = state.template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* svelocity       = state.template getField<Lin::Vector<dim>>("velocity");

        Field<Lin::Vector<dim>>* position       = nodeList->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* velocity       = nodeList->template getField<Lin::Vector<dim>>("velocity");

        position->copyValues(fposition);
        velocity->copyValues(fvelocity);
        sposition->copyValues(fposition);
        svelocity->copyValues(fvelocity);
    }
};
