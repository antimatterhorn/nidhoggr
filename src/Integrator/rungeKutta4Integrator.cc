#include "integrator.hh"

template <typename T>
class RungeKutta4Integrator : public Integrator<T> {
public:
    using DerivativeFunction = std::function<Field<T>(T, const Field<T>&)>;
    RungeKutta4Integrator(const DerivativeFunction& derivativeFunc) : Integrator<T>(derivativeFunc) {}

    // Integrate method using Runge-Kutta 4
    Field<T> 
    integrate(const Field<T>& initialState, double t, double dt) override {
        Field<T> k1 = this->DxDt(t, initialState);
        Field<T> k2 = this->DxDt(t + dt / 2, initialState + k1*(dt / 2));
        Field<T> k3 = this->DxDt(t + dt / 2, initialState + k2*(dt / 2));
        Field<T> k4 = this->DxDt(t + dt, initialState + k3 * dt);

        Field<T> nextState = initialState + (k1 + k2*2 + k3*2 + k4) * (dt / 6);

        return nextState;
    }
};
