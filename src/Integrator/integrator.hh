#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>
#include <cstddef>

// Define a type alias for the derivative function
using DerivativeFunction = std::function<std::vector<double>(double, const std::vector<double>&)>;

class Integrator {
public:
    DerivativeFunction DxDt;
    
    Integrator (const DerivativeFunction& derivativeFunc) : DxDt(derivativeFunc) {}

    ~Integrator() {}

    virtual std::vector<double> integrate(std::vector<double> initialState, double t, double dt) {
        // fwd euler
        std::vector<double> nextState = addVectors(initialState,scalarMultiply(DxDt(t,initialState),dt));
        return nextState;
    }

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
private:


};

#endif // INTEGRATOR_HH