#include "integrator.hh"

class RungeKutta4Integrator : public Integrator {
public:
    RungeKutta4Integrator(const State& state) : Integrator(state) {}

    // Integrate method using Runge-Kutta 4
    template <typename T>
    Field<T> 
    integrate(const Field<T>& initialState, 
                const std::function<Field<T>(T, const Field<T>&)>& deriv, 
                double t, double dt) {
        Field<T> k1 = deriv(t, initialState);
        Field<T> k2 = deriv(t + dt / 2, initialState + k1*(dt / 2));
        Field<T> k3 = deriv(t + dt / 2, initialState + k2*(dt / 2));
        Field<T> k4 = deriv(t + dt, initialState + k3 * dt);

        Field<T> nextState = initialState + (k1 + k2*2 + k3*2 + k4) * (dt / 6);

        return nextState;
    }
};
