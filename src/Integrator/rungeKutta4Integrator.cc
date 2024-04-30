
#include "integrator.hh"
#include <cstddef>

class RungeKutta4Integrator : public Integrator {
public:
    RungeKutta4Integrator(const DerivativeFunction& derivativeFunc) : Integrator(derivativeFunc) {}

    // Integrate method using Runge-Kutta 4
    std::vector<double> integrate(std::vector<double> initialState, double t, double dt) {
        std::vector<double> k1 = this->DxDt(t, initialState);
        std::vector<double> k2 = this->DxDt(t + dt / 2, addVectors(initialState, scalarMultiply(k1, dt / 2)));
        std::vector<double> k3 = this->DxDt(t + dt / 2, addVectors(initialState, scalarMultiply(k2, dt / 2)));
        std::vector<double> k4 = this->DxDt(t + dt, addVectors(initialState, scalarMultiply(k3, dt)));

        std::vector<double> nextState = addVectors(initialState, scalarMultiply(addVectors(addVectors(k1, scalarMultiply(k2, 2)), addVectors(k3, k4)), dt / 6));

        return nextState;
    }

private:
    

    std::vector<double> addVectors(const std::vector<double>& v1, const std::vector<double>& v2) {
        std::vector<double> result;
        for (size_t i = 0; i < v1.size(); ++i) {
            result.push_back(v1[i] + v2[i]);
        }
        return result;
    }

    std::vector<double> scalarMultiply(const std::vector<double>& v, double scalar) {
        std::vector<double> result;
        for (double val : v) {
            result.push_back(val * scalar);
        }
        return result;
    }
};
