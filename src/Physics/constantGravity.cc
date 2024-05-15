#include "physics.hh"
#include <iostream>

template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
    double dtmin;
public:
    ConstantGravity() {}

    ConstantGravity(NodeList* nodeList, PhysicalConstants& constants, Lin::Vector<dim>& gravityVector) : 
        Physics<dim>(nodeList,constants),
        gravityVector(gravityVector) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("acceleration");
        for (int i=0; i<numNodes; ++i)
            nodeList->getField<Lin::Vector<dim>>("acceleration")->setValue(i,gravityVector);
        
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        std::shared_ptr<Field<Lin::Vector<dim>>> positionSharedPtr(position);
        State<dim>* state = &this->state;        
        state->template addField<Lin::Vector<dim>>(positionSharedPtr);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        std::shared_ptr<Field<Lin::Vector<dim>>> velocitySharedPtr(velocity);
        state->template addField<Lin::Vector<dim>>(velocitySharedPtr);
    }

    ~ConstantGravity() {}

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double t) override {
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* position       = initialState->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* acceleration   = nodeList->getField<Lin::Vector<dim>>("acceleration");
        Field<Lin::Vector<dim>>* velocity       = initialState->template getField<Lin::Vector<dim>>("velocity");

        Field<Lin::Vector<dim>>* dxdt           = deriv.template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* dvdt           = deriv.template getField<Lin::Vector<dim>>("velocity");
        
        dxdt->copyValues(velocity);
        dxdt->operator+(*acceleration*t);
        dvdt->copyValues(acceleration);

        dtmin = 1e30;
        #pragma omp parllel for
        for (int i=0; i<numNodes ; ++i) {
            double amag = acceleration->getValue(i).mag2();
            double vmag = velocity->getValue(i).mag2();
            dtmin = std::min(dtmin,vmag/amag);
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

        Field<Lin::Vector<dim>>* fposition       = finalState->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* fvelocity       = finalState->template getField<Lin::Vector<dim>>("velocity");

        Field<Lin::Vector<dim>>* position       = nodeList->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* velocity       = nodeList->template getField<Lin::Vector<dim>>("velocity");

        position->copyValues(fposition);
        velocity->copyValues(fvelocity);
    }
};
