#include "physics.hh"
#include <iostream>

template <int dim>
class Kinetics : public Physics<dim> {
protected:
    double dtmin;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    Kinetics(NodeList* nodeList, PhysicalConstants& constants) :
        Physics<dim>(nodeList,constants) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Vector>("acceleration") == nullptr)
            nodeList->insertField<Vector>("acceleration");
        if (nodeList->getField<double>("radius") == nullptr)
            nodeList->insertField<double>("radius");

        VectorField* position = nodeList->getField<Vector>("position");
        State<dim>* state = &this->state;
        state->template addField<Vector>(position);

        // kinetics will operate solely on the spatial derivative. velocities merely change direction
        // i.e. there is no acceleration term for this physics package so no dvdt
    }

    ~Kinetics() {}

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

        ScalarField* mass           = nodeList->getField<double>("mass");
        ScalarField* radius         = nodeList->getField<double>("radius");
        VectorField* position       = initialState->template getField<Vector>("position");
        VectorField* velocity       = nodeList->getField<Vector>("velocity");

        VectorField* dxdt           = deriv.template getField<Vector>("position");

        for (int i = 0; i < numNodes - 1; ++i) {
            for (int j = i + 1; j < numNodes; ++j) {
                double mi = mass->getValue(i);
                double ri = radius->getValue(i);
                Vector vi = velocity->getValue(i);
                double mj = mass->getValue(j);
                double rj = radius->getValue(j);
                Vector vj = velocity->getValue(j);

                double pi = mi*vi.magnitude();
                double pj = mj*vj.magnitude();
            }
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

        VectorField* fposition  = finalState->template getField<Vector>("position");
        VectorField* position   = nodeList->template getField<Vector>("position");

        position->copyValues(fposition);
    }
};
