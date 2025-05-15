#include "integrator.hh"

template <int dim>
class CrankNicolsonIntegrator : public Integrator<dim> {
public:
    CrankNicolsonIntegrator(std::vector<Physics<dim>*> packages, double dtmin, bool verbose = false) : 
        Integrator<dim>(packages, dtmin, verbose) {}

    ~CrankNicolsonIntegrator() {}

    virtual void Step() override {
        std::vector<Physics<dim>*> packages = this->packages;

        if (this->cycle == 0)
            for (Physics<dim>* physics : packages)
                physics->ZeroTimeInitialize();

        double time = this->time;
        double dt = this->dt;

        for (Physics<dim>* physics : packages) {
            physics->PreStepInitialize();
            physics->ApplyBoundaries();

            State<dim>* state = physics->getState();
            State<dim> k1(state->size());
            State<dim> k2(state->size());

            k1.ghost(state);
            k2.ghost(state);

            // Evaluate at current state
            physics->EvaluateDerivatives(state, k1, time, 0.0);

            // Predictor step: Euler
            State<dim> predicted(state->size());
            predicted.clone(state);
            predicted += k1 * dt;

            // Corrector iterations (fixed-point)
            const int maxIterations = 3;
            for (int iter = 0; iter < maxIterations; ++iter) {
                // Evaluate derivatives at predicted state (future time)
                physics->EvaluateDerivatives(&predicted, k2, time + dt, 0.0);

                // Average derivative: (k1 + k2) / 2
                State<dim> avg(state->size());
                avg.clone(&k1);
                avg += k2;
                avg *= 0.5 * dt;

                // Update predicted state
                predicted.clone(state);
                predicted += avg;
            }

            physics->ApplyBoundaries();
            physics->FinalizeStep(&predicted);
        }

        this->time += dt;
        this->cycle += 1;

        this->VoteDt();
    }
};
