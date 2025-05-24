#include "physics.hh"
#include <iostream>

template <int dim>
class SimplePhysics : public Physics<dim> {
protected:

public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    SimplePhysics(NodeList* nodeList,
                        PhysicalConstants& constants) :
        Physics<dim>(nodeList,constants) {

        int numNodes = nodeList->size();
        this->template EnrollFields<double>({"y"});
        this->template EnrollStateFields<double>({"y"});
    }

    ~SimplePhysics() {}

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;

        ScalarField* sy       = state.template getField<double>("y");
        ScalarField* y        = nodeList->template getField<double>("y");

        sy->copyValues(y);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        // extremely simple ode, y(t) = 10*t^2 - 1/2*t^3  -> y'=20*t - 3/2*t^2

        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        ScalarField* y        = initialState->template getField<double>("y");
        ScalarField* dydt     = deriv.template getField<double>("y");

        #pragma omp parallel for
        for (int i=0; i<numNodes ; ++i) {
            dydt->setValue(i,20.0*(time+dt) - 1.5*(time+dt)*(time+dt));
        }
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;

        ScalarField* sy       = state.template getField<double>("y");
        ScalarField* fy       = finalState->template getField<double>("y");
        ScalarField* y        = nodeList->template getField<double>("y");

        sy->copyValues(fy);
        y->copyValues(fy);
    }
};
