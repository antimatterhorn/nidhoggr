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
        this->EnrollScalars({"mass", "radius"});
        this->EnrollVectors({"acceleration", "velocity", "position"});
        this->EnrollStateVectors({"position"});

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
            double local_dtmin = 1e30;

            #pragma omp parallel for reduction(min:local_dtmin)
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

                    if (rij.magnitude() < (si+sj) && vij*rij<0) { //collision
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
                local_dtmin = std::min(local_dtmin,0.25*si/vi.magnitude());
            }
            dtmin = local_dtmin;
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

    virtual std::string name() const override { return "kinetics"; }
    virtual std::string description() const override {
        return "The kinetics physics package for particles"; }
};
