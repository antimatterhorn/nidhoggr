
```mermaid
classDiagram
    Physics <|-- SimplePhysics
    Physics <|-- PointSourceGravity
    Physics <|-- ConstantGravity
    Physics <|-- NBodyGravity
    Physics <|-- WaveEquation
    Physics <|-- Hydro
    Physics : +NodeList* nodeList
    Physics : +PhysicalConstants& constants
    Physics : VerifyFields(NodeList* nodeList)
    Physics : ZeroTimeInitialize()
    Physics : PrestepInitialize()
    Physics : EvaluateDerivatives(State* initialState, State<dim>& deriv, double time, double dt)
    Physics : FinalizeStep(State* finalState)
    class PointSourceGravity{
        +Vector pointSourceLocation
        +double pointSourceMass
    }
    class ConstantGravity{
        +Vector gravity
    }
    class NBodyGravity{
        +double plummerLength
    }
    class Hydro{
        +EquationOfState* eos
    }
    class SimplePhysics{
        
    }
    class WaveEquation{
        +Grid* grid
        +double soundSpeed
    }
    WaveEquation o -- _WaveEquation
    class _WaveEquation{
        +Grid* grid
        +string depthMap
    }
    class RockPaperScissors{
        +Grid* grid
        +double A
        +double D
    }
    Hydro <| -- GridHydroHLL
    class GridHydroHLL{
        +Grid* grid
    }
    Hydro <| -- EulerHydro
    class EulerHydro{
        +Grid* grid
    }
```