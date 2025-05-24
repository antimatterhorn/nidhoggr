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

    ConstantGravity(NodeList* nodeList, PhysicalConstants& constants, Vector& gravityVector) :
        Physics<dim>(nodeList,constants),
        gravityVector(gravityVector) {

        int numNodes = nodeList->size();
        this->EnrollVectors({"acceleration", "velocity", "position"});
        
        for (int i=0; i<numNodes; ++i)
            nodeList->getField<Vector>("acceleration")->setValue(i,gravityVector);

        this->EnrollStateVectors({"velocity", "position"});
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

        double local_dtmin = 1e30;

        #pragma omp parallel for reduction(min:local_dtmin)
        for (int i=0; i<numNodes ; ++i) {
            double amag = acceleration->getValue(i).mag2();
            double vmag = velocity->getValue(i).mag2();
            local_dtmin = std::min(local_dtmin,vmag/amag);
        }
        dtmin = local_dtmin;
        this->lastDt = dt;
    }

    virtual double
    EstimateTimestep() const override {
        double timestepCoefficient = 1e-4; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        PushState(finalState);
    }

    virtual void
    PushState(const State<dim>* stateToPush) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        State<dim> state = this->state;

        VectorField* position       = nodeList->template getField<Vector>("position");
        VectorField* velocity       = nodeList->template getField<Vector>("velocity");

        VectorField* fposition       = stateToPush->template getField<Vector>("position");
        VectorField* fvelocity       = stateToPush->template getField<Vector>("velocity");

        position->copyValues(fposition);
        velocity->copyValues(fvelocity);

        if (stateToPush != &(state))
        {
            VectorField* sposition       = state.template getField<Vector>("position");
            VectorField* svelocity       = state.template getField<Vector>("velocity");
            sposition->copyValues(fposition);
            svelocity->copyValues(fvelocity);
        }
    }

    virtual std::string name() const override { return "constantGravity"; }
    virtual std::string description() const override {
        return "Constant acceleration"; }
};
