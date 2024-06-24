#include "physics.hh"
#include <iostream>

template <int dim>
class PointSourceGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> pointSourceLocation;
    double pointSourceMass;
    double dtmin;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    PointSourceGravity(NodeList* nodeList,
                        PhysicalConstants& constants,
                        Vector& pointSourceLocation,
                        double pointSourceMass) :
        Physics<dim>(nodeList,constants),
        pointSourceLocation(pointSourceLocation),
        pointSourceMass(pointSourceMass) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Vector>("acceleration") == nullptr)
            nodeList->insertField<Vector>("acceleration");

        State<dim>* state = &this->state;
        VectorField* position = nodeList->getField<Vector>("position");
        state->template addField<Vector>(position);
        VectorField* velocity = nodeList->getField<Vector>("velocity");
        state->template addField<Vector>(velocity);
    }

    ~PointSourceGravity() {}

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        //compute accelerations
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        VectorField* position       = initialState->template getField<Vector>("position");
        VectorField* acceleration   = nodeList->getField<Vector>("acceleration");
        // ^ this field is just for reference and isn't actually used to calculate anything
        VectorField* velocity       = initialState->template getField<Vector>("velocity");

        VectorField* dxdt           = deriv.template getField<Vector>("position");
        VectorField* dvdt           = deriv.template getField<Vector>("velocity");

        dtmin = 1e30;
        #pragma omp parallel for
        for (int i=0; i<numNodes ; ++i) {
            Vector pos = position->getValue(i);
            Vector r = (pointSourceLocation - pos);
            Vector a = pointSourceMass*constants.G()/(r.mag2())*r.normal();
            Vector v = velocity->getValue(i);
            acceleration->setValue(i,a);
            double amag = a.mag2();
            double vmag = v.mag2();
            dtmin = std::min(dtmin,vmag/amag);
            dxdt->setValue(i,v+dt*a);
            dvdt->setValue(i,a);
        }

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
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        State<dim> state = this->state;

        VectorField* fposition       = finalState->template getField<Vector>("position");
        VectorField* fvelocity       = finalState->template getField<Vector>("velocity");

        VectorField* sposition       = state.template getField<Vector>("position");
        VectorField* svelocity       = state.template getField<Vector>("velocity");

        VectorField* position       = nodeList->template getField<Vector>("position");
        VectorField* velocity       = nodeList->template getField<Vector>("velocity");

        position->copyValues(fposition);
        velocity->copyValues(fvelocity);
        sposition->copyValues(fposition);
        svelocity->copyValues(fvelocity);
    }
};
