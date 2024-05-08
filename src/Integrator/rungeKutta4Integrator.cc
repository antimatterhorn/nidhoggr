#include "integrator.hh"

template <int dim>
class RungeKutta4Integrator : public Integrator<dim> {
protected:
public:
    RungeKutta4Integrator(Physics<dim>* physics) : Integrator<dim>(physics) {}

    ~RungeKutta4Integrator() {}

    virtual void
    Step(double dt) override {
        Physics<dim>* physics = this->physics;
        double time = this->time;
        
        for (FieldBase* field : physics->derivFields) {
            if (typeid(*field) == typeid(Field<double>)) {
                Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
                if (doubleField) {
                    Field<double> k1,k2,k3,k4;
                    k1 = this->Derivative(doubleField,time,0);
                    Field<double> interim = Field<double>(doubleField->getNameString(),doubleField->size());
                    interim.copyValues(*doubleField + k1*(dt/2.0));
                    k2 = this->Derivative(&interim,time,dt/2.0);
                    interim.copyValues(*doubleField + k2*(dt/2.0));
                    k3 = this->Derivative(&interim,time,dt/2.0);
                    interim.copyValues(*doubleField + k3*dt);
                    k4 = this->Derivative(&interim,time,dt);
                    Field<double> nextState = *doubleField + (k1 + k2*2.0 + k3*2.0 + k4) * (dt / 6.0);
                    doubleField->copyValues(nextState);
                }
            } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
                Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                if (vectorField) {
                    Field<Lin::Vector<dim>> k1,k2,k3,k4;
                    k1 = this->Derivative(vectorField,time,0);
                    Field<Lin::Vector<dim>> interim = Field<Lin::Vector<dim>>(vectorField->getNameString(),vectorField->size());
                    interim.copyValues(*vectorField + k1*(dt/2.0));
                    k2 = this->Derivative(&interim,time,dt/2.0);
                    interim.copyValues(*vectorField + k2*(dt/2.0));
                    k3 = this->Derivative(&interim,time,dt/2.0);
                    interim.copyValues(*vectorField + k3*dt);
                    k4 = this->Derivative(&interim,time,dt);
                    Field<Lin::Vector<dim>> nextState = *vectorField + (k1 + k2*2.0 + k3*2.0 + k4) * (dt / 6.0);
                    vectorField->copyValues(nextState);
                }
            }
        }
        this->time += dt;
    }

    virtual
    Field<double> 
    Derivative(const Field<double>* initialState, 
                double t, double dt) override {
        Field<double> deriv = Field<double>("deriv"+initialState->getNameString(),initialState->size());
        Physics<dim>* physics = this->physics;
        physics->EvaluateDerivatives(initialState,deriv,dt);
        return deriv;
    }

    virtual
    Field<Lin::Vector<dim>> 
    Derivative(const Field<Lin::Vector<dim>>* initialState, 
                double t, double dt) override {
        Field<Lin::Vector<dim>> deriv = Field<Lin::Vector<dim>>("deriv"+initialState->getNameString(),initialState->size());
        Physics<dim>* physics = this->physics;
        physics->EvaluateDerivatives(initialState,deriv,dt);
        return deriv;
    }

    //Integrate method using Runge-Kutta 4
    // virtual
    // Field<double> 
    // integrate(const Field<double>* initialState, 
    //             double t, double dt) override {
    //     Physics* physics = this->physics;
    //     Field<double> k1 = Field<double>("k1"+initialState->getNameString(),initialState->size());
    //     Field<double> k2 = Field<double>("k2"+initialState->getNameString(),initialState->size());
    //     Field<double> k3 = Field<double>("k3"+initialState->getNameString(),initialState->size());
    //     Field<double> k4 = Field<double>("k4"+initialState->getNameString(),initialState->size());
    //     physics->EvaluateDerivatives(initialState,k1,t);
    //     Field<double> interim = initialState + k1*(dt/2.0);
    //     physics->EvaluateDerivatives(interim,k2,t+dt/2.0);
    //     interim = initialState + k2*(dt/2.0);
    //     physics->EvaluateDerivatives(interim,k3,t+dt/2.0);
    //     Field<double> k3 = deriv(t + dt / 2, initialState + k2*(dt / 2));
    //     Field<double> k4 = deriv(t + dt, initialState + k3 * dt);

    //     Field<double> nextState = initialState + (k1 + k2*2 + k3*2 + k4) * (dt / 6);

    //     return nextState;
    // }

    // virtual
    // Field<Lin::Vector<dim>> 
    // integrate(const Field<Lin::Vector<dim>>* initialState, 
    //             double t, double dt) override {
    //     Field<Lin::Vector<dim>> k1 = deriv(t, initialState);
    //     Field<Lin::Vector<dim>> k2 = deriv(t + dt / 2, initialState + k1*(dt / 2));
    //     Field<Lin::Vector<dim>> k3 = deriv(t + dt / 2, initialState + k2*(dt / 2));
    //     Field<Lin::Vector<dim>> k4 = deriv(t + dt, initialState + k3 * dt);

    //     Field<Lin::Vector<dim>> nextState = initialState + (k1 + k2*2 + k3*2 + k4) * (dt / 6);

    //     return nextState;
    // }
};
