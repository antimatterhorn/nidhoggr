#include "physics.hh"
#include <iostream>

// Physics/implicitPhysics.cc 
// This is a simple example of a physics class that requires implicit time integration.
// y' = y + t^2
template <int dim>
class ImplicitPhysics : public Physics<dim> {
protected:

public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    ImplicitPhysics(NodeList* nodeList,
                        PhysicalConstants& constants) :
        Physics<dim>(nodeList,constants) {

        int numNodes = nodeList->size();
        this->template EnrollFields<double>({"y"});
        this->template EnrollStateFields<double>({"y"});
    }

    ~ImplicitPhysics() {}

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;

        ScalarField* sy = state.template getField<double>("y");
        ScalarField* y  = nodeList->template getField<double>("y");

        sy->copyValues(y);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        const double evalTime = time + dt;

        ScalarField* y    = initialState->template getField<double>("y");
        ScalarField* dydt = deriv.template getField<double>("y");

        for (int i = 0; i < numNodes; ++i) {
            const double yi = (*y)[i];
            dydt->setValue(i, yi + evalTime * evalTime);
        }
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;

        ScalarField* sy = state.template getField<double>("y");
        ScalarField* fy = finalState->template getField<double>("y");
        ScalarField* y  = nodeList->template getField<double>("y");

        sy->copyValues(fy);
        y->copyValues(fy);
    }
};
