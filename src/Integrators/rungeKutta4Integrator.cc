#include "integrator.hh"

template <int dim>
class RungeKutta4Integrator : public Integrator<dim> {
protected:

public:
    RungeKutta4Integrator(Physics<dim>* physics, double dtmin, std::vector<Boundaries<dim>*> boundaries) : 
        Integrator<dim>(physics,dtmin,boundaries) {}

    ~RungeKutta4Integrator() {}

    virtual void
    Step() override {
        Physics<dim>* physics = this->physics;

        if (this->cycle == 0)
            physics->ZeroTimeInitialize();

        physics->PreStepInitialize();

        std::vector<Boundaries<dim>*> boundaries = this->boundaries;
        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries(); 

        double time = this->time;
        double dt = this->dt;

        State<dim>* state = physics->getState();
        State<dim> interim(state->size());
        State<dim> k1(state->size());
        State<dim> k2(state->size());
        State<dim> k3(state->size());
        State<dim> k4(state->size());

        k1.ghost(state);
        k2.ghost(state);
        k3.ghost(state);
        k4.ghost(state);

        physics->EvaluateDerivatives(state,k1,time,0);

        interim.clone(state);
        interim+=k1*(dt/2.0);  
        physics->EvaluateDerivatives(&interim,k2,time,dt/2.0);

        interim.clone(state);
        interim+=k2*(dt/2.0);
        physics->EvaluateDerivatives(&interim,k3,time,dt/2.0);

        interim.clone(state);
        interim+=k3*dt;
        physics->EvaluateDerivatives(&interim,k4,time,dt);

        k2*=2.0;
        k3*=2.0;
        k1+=k2;
        k1+=k3;
        k1+=k4;
        k1*=(dt/6.0);

        State<dim> newState(state->size());
        newState.clone(state);
        newState += k1;

        physics->FinalizeStep(&newState);
     
        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries();

        this->time += dt;
        this->cycle += 1;

        double newdt = physics->EstimateTimestep();
        this->dt = std::max(newdt,this->dtmin);
    }
};
