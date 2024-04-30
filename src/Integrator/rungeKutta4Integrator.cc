
#include "integrator.hh"


class RungeKutta4Integrator : public Integrator {
public:
    RungeKutta4Integrator(const DerivativeFunction& derivativeFunc) : Integrator(derivativeFunc) {}

    // Integrate method using Runge-Kutta 4
    std::vector<double> integrate(std::vector<double> initialState, double t, double dt) override {
        std::vector<double> k1 = this->DxDt(t, initialState);
        std::vector<double> k2 = this->DxDt(t + dt / 2, addVectors(initialState, scalarMultiply(k1, dt / 2)));
        std::vector<double> k3 = this->DxDt(t + dt / 2, addVectors(initialState, scalarMultiply(k2, dt / 2)));
        std::vector<double> k4 = this->DxDt(t + dt, addVectors(initialState, scalarMultiply(k3, dt)));

        std::vector<double> nextState = addVectors(initialState, scalarMultiply(addVectors(addVectors(k1, scalarMultiply(k2, 2)), addVectors(k3, k4)), dt / 6));

        return nextState;
    }

private:
    


};
