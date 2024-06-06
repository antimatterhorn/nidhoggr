#include "physics.hh"
#include <iostream>

template <int dim>
class ConstantGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> gravityVector;
    double dtmin;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    ConstantGravity() {}

    ConstantGravity(NodeList* nodeList, PhysicalConstants& constants, Vector& gravityVector) :
        Physics<dim>(nodeList,constants),
        gravityVector(gravityVector) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Vector>("acceleration") == nullptr)
            nodeList->insertField<Vector>("acceleration");
        for (int i=0; i<numNodes; ++i)
            nodeList->getField<Vector>("acceleration")->setValue(i,gravityVector);

        VectorField* position = nodeList->getField<Vector>("position");
        State<dim>* state = &this->state;
        state->template addField<Vector>(position);
        VectorField* velocity = nodeList->getField<Vector>("velocity");
        state->template addField<Vector>(velocity);
    }

    ~ConstantGravity() {}

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        VectorField* position       = initialState->template getField<Vector>("position");
        VectorField* acceleration   = nodeList->getField<Vector>("acceleration");
        VectorField* velocity       = initialState->template getField<Vector>("velocity");

        VectorField* dxdt           = deriv.template getField<Vector>("position");
        VectorField* dvdt           = deriv.template getField<Vector>("velocity");

        dxdt->copyValues(velocity);
        dxdt->operator+(*acceleration*dt);
        dvdt->copyValues(acceleration);

        dtmin = 1e30;
        #pragma omp parallel for
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

        VectorField* fposition       = finalState->template getField<Vector>("position");
        VectorField* fvelocity       = finalState->template getField<Vector>("velocity");

        VectorField* position       = nodeList->template getField<Vector>("position");
        VectorField* velocity       = nodeList->template getField<Vector>("velocity");

        position->copyValues(fposition);
        velocity->copyValues(fvelocity);
    }
};
