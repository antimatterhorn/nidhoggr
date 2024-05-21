```mermaid
classDiagram
Integrator <|-- RungeKutta2Integrator
Integrator <|-- RungeKutta4Integrator
Integrator : +Physics* physics
Integrator : +double dtmin
Integrator : +stdvector boundaries
Integrator : Step()
Integrator : double Time()
Integrator : int Cycle()
Integrator : double Dt()
```