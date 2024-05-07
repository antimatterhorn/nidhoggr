#include "integrator.hh"

template <int dim>
class RungeKutta4Integrator : public Integrator<dim> {
public:
    RungeKutta4Integrator(const Physics<dim>* physics) : Integrator<dim>(physics) {}

    ~RungeKutta4Integrator() {}

    //Integrate method using Runge-Kutta 4
    virtual
    Field<double> 
    integrate(const Field<double>& initialState, 
                const std::function<Field<double>(double, 
                const Field<double>&)>& deriv, 
                double t, double dt) override {
        Field<double> k1 = deriv(t, initialState);
        Field<double> k2 = deriv(t + dt / 2, initialState + k1*(dt / 2));
        Field<double> k3 = deriv(t + dt / 2, initialState + k2*(dt / 2));
        Field<double> k4 = deriv(t + dt, initialState + k3 * dt);

        Field<double> nextState = initialState + (k1 + k2*2 + k3*2 + k4) * (dt / 6);

        return nextState;
    }

    virtual
    Field<Lin::Vector<dim>> 
    integrate(const Field<Lin::Vector<dim>>& initialState, 
                const std::function<Field<Lin::Vector<dim>>(Lin::Vector<dim>, 
                const Field<Lin::Vector<dim>>&)>& deriv, 
                double t, double dt) override {
        Field<Lin::Vector<dim>> k1 = deriv(t, initialState);
        Field<Lin::Vector<dim>> k2 = deriv(t + dt / 2, initialState + k1*(dt / 2));
        Field<Lin::Vector<dim>> k3 = deriv(t + dt / 2, initialState + k2*(dt / 2));
        Field<Lin::Vector<dim>> k4 = deriv(t + dt, initialState + k3 * dt);

        Field<Lin::Vector<dim>> nextState = initialState + (k1 + k2*2 + k3*2 + k4) * (dt / 6);

        return nextState;
    }
};
