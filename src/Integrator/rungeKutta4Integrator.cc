#include "integrator.hh"

class RungeKutta4Integrator : public Integrator {
public:
    RungeKutta4Integrator(const DerivativeFunction& derivativeFunc) : Integrator(derivativeFunc) {}

    // Integrate method using Runge-Kutta 4
    Field<double> integrate(const Field<double>& initialState, double t, double dt) override {
        Field<double> k1 = this->DxDt(t, initialState);
        Field<double> k2 = this->DxDt(t + dt / 2, initialState + k1 * (dt / 2));
        Field<double> k3 = this->DxDt(t + dt / 2, initialState + k2 * (dt / 2));
        Field<double> k4 = this->DxDt(t + dt, initialState + k3 * dt);

        Field<double> nextState = initialState + (k1 + k2 * 2 + k3 * 2 + k4) * (dt / 6);

        return nextState;
    }
};
