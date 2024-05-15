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

        physics->PreStepInitialize();
        
        double time = this->time;
        double dt = this->dt;

        State<dim>* state = physics->getState();
        State<dim> k1(state->size());
        k1.ghost(state);
        physics->EvaluateDerivatives(state,k1,0);

        State<dim> interim(state->size());
        interim.clone(state);
        interim+=k1*(dt/2.0);

        State<dim> k2(state->size());
        k2.clone(&interim);
        physics->EvaluateDerivatives(&interim,k2,dt/2.0);
        interim.clone(state);
        interim+=k2*(dt/2.0);

        State<dim> k3(state->size());
        k3.clone(&interim);
        physics->EvaluateDerivatives(&interim,k3,dt/2.0);
        interim.clone(state);
        interim+=k3*dt;

        State<dim> k4(state->size());
        k4.clone(&interim);
        physics->EvaluateDerivatives(&interim,k4,dt);

        k2*=2.0;
        k3*=3.0;
        k1+=k2;
        k1+=k3;
        k1+=k4;
        k1*=(dt/6.0);

        State<dim> newState(state->size());
        newState.clone(state);
        newState += k1;

        physics->FinalizeStep(&newState);

        // for (FieldBase* field : state->fields) {
        //     if (typeid(*field) == typeid(Field<double>)) {
        //         Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
        //         if (doubleField) {
        //             Field<double> k1,k2,k3,k4;
        //             k1 = this->Derivative(doubleField,time,0);
        //             Field<double> interim = Field<double>(doubleField->getNameString(),doubleField->size());
        //             interim.copyValues(*doubleField + k1*(dt/2.0));
        //             k2 = this->Derivative(&interim,time,dt/2.0);
        //             interim.copyValues(*doubleField + k2*(dt/2.0));
        //             k3 = this->Derivative(&interim,time,dt/2.0);
        //             interim.copyValues(*doubleField + k3*dt);
        //             k4 = this->Derivative(&interim,time,dt);
        //             Field<double> nextState = *doubleField + (k1 + k2*2.0 + k3*2.0 + k4) * (dt / 6.0);
        //             doubleField->copyValues(nextState);
        //         }
        //     } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
        //         Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
        //         if (vectorField) {
        //             Field<Lin::Vector<dim>> k1,k2,k3,k4;
        //             k1 = this->Derivative(vectorField,time,0);
        //             Field<Lin::Vector<dim>> interim = Field<Lin::Vector<dim>>(vectorField->getNameString(),vectorField->size());
        //             interim.copyValues(*vectorField + k1*(dt/2.0));
        //             k2 = this->Derivative(&interim,time,dt/2.0);
        //             interim.copyValues(*vectorField + k2*(dt/2.0));
        //             k3 = this->Derivative(&interim,time,dt/2.0);
        //             interim.copyValues(*vectorField + k3*dt);
        //             k4 = this->Derivative(&interim,time,dt);
        //             Field<Lin::Vector<dim>> nextState = *vectorField + (k1 + k2*2.0 + k3*2.0 + k4) * (dt / 6.0);
        //             vectorField->copyValues(nextState);
        //         }
        //     }
        // }
        std::vector<Boundaries<dim>*> boundaries = this->boundaries;
        if(boundaries.size() > 0)
            for(Boundaries<dim>* bounds : boundaries)
                bounds->ApplyBoundaries();

        this->time += dt;
        this->cycle += 1;

        double newdt = physics->EstimateTimestep();
        this->dt = std::max(newdt,this->dtmin);
    }
};
