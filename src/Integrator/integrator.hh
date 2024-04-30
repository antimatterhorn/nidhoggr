#ifndef INTEGRATOR_HH
#define INTEGRATOR_HH

#include <functional>
#include <vector>

// Define a type alias for the derivative function
using DerivativeFunction = std::function<std::vector<double>(double, const std::vector<double>&)>;

class Integrator {
public:
    DerivativeFunction DxDt;
    
    Integrator (const DerivativeFunction& derivativeFunc) : DxDt(derivativeFunc) {}

    ~Integrator() {}
private:
    

};

#endif // INTEGRATOR_HH