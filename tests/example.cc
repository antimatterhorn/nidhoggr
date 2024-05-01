#include <iostream>
#include "rungeKutta4Integrator.cc"
#include "../DataBase/field.hh"

// Define the derivative function for the differential equation
Field<double> derivative(double t, const Field<double>& y) {
    // Example differential equation: dy/dt = y^2
    Field<double> dydt("dydt");
    for (size_t i = 0; i < y.size(); ++i) {
        dydt.addValue(y[i] * y[i]);
    }
    return dydt;
}

int main() {
    // Define the initial state
    std::cout << "creating state";
    Field<double> initialState("y");

    
    initialState.addValue(1.0); // Initial value of y

    // Define the time parameters
    double t = 0.0;
    double dt = 0.1; // Time step

    // Create an instance of the Runge Kutta 4 integrator
    RungeKutta4Integrator<double> integrator(derivative);
    std::cout << "creating integrator";
    // Integrate the differential equation
    Field<double> nextState = integrator.integrate(initialState, t, dt);
    std::cout << "creating field";
    // Output the result
    std::cout << "Next state: ";
    for (size_t i = 0; i < nextState.getSize(); ++i) {
        std::cout << nextState[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
