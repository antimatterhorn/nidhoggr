#include "physics.hh"
#include <iostream>

template <int dim>
class Kinetics : public Physics<dim> {
protected:
    double dtmin;
    double timeVisited;
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
        if (time != timeVisited) {
            NodeList* nodeList = this->nodeList;
            PhysicalConstants constants = this->constants;
            int numNodes = nodeList->size();

            ScalarField* mass           = nodeList->getField<double>("mass");
            ScalarField* radius         = nodeList->getField<double>("radius");
            VectorField* position       = initialState->template getField<Vector>("position");
            VectorField* velocity       = nodeList->getField<Vector>("velocity");

            //VectorField* dxdt           = deriv.template getField<Vector>("position");

            for (int i = 0; i < numNodes - 1; ++i) {
                dtmin = 1e30;
                for (int j = i + 1; j < numNodes; ++j) {
                    Vector ri = position->getValue(i);
                    Vector rj = position->getValue(j);
                    double si = radius->getValue(i);
                    double sj = radius->getValue(j);
                    Vector vi = velocity->getValue(i);
                    Vector vj = velocity->getValue(j);

                    Vector vij = vi-vj;
                    Vector rij = ri-rj;

                    if (rij.magnitude() < (si+sj) && vij*rij<0) {
                        double mi = mass->getValue(i);
                        double mj = mass->getValue(j);

                        Vector vip = vi - 2*mj/(mi+mj)*(vij*rij/rij.mag2())*rij;
                        Vector vjp = vj + 2*mi/(mi+mj)*(vij*rij/rij.mag2())*rij;

                        velocity->setValue(i,vip);
                        velocity->setValue(j,vjp);
                        // dxdt->setValue(i,vip);
                        // dxdt->setValue(j,vjp);

                        continue;  // skip this i (1 collision only please!)
                    }
                }
                Vector vi = velocity->getValue(i);
                double si = radius->getValue(i);
                dtmin = std::min(dtmin,0.25*si/vi.magnitude());
            }
        }
        

        timeVisited = time;
        this->lastDt = dt;
    }

    virtual double
    EstimateTimestep() const override {
        double timestepCoefficient = 0.25; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {

    }
};
