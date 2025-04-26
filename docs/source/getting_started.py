from nidhoggr import *
from Animation import *

if __name__ == "__main__":
    constants = MKS()
    numNodes = 1
    myNodeList = NodeList(numNodes)
    # Node_created
    gravVec = Vector2d(0, -9.8)

    constantGravity = ConstantGravity2d(myNodeList, constants, gravVec)
    kinetics = Kinetics2d(myNodeList,constants)
    packages = [constantGravity,kinetics]
    # Packages_created
    integrator = RungeKutta4Integrator2d(packages=packages, dtmin=0.01)
    print(integrator)
    # Integrator_created
    rad     = myNodeList.getFieldDouble("radius")
    mass    = myNodeList.getFieldDouble("mass")
    vel     = myNodeList.getFieldVector2d("velocity")
    pos     = myNodeList.getFieldVector2d("position")
    
    mass.setValue(0, 1)
    rad.setValue(0, 0.01)
    vel.setValue(0, Vector2d(5,5))
    pos.setValue(0, Vector2d(0,0))
    # Initial_conditions_set
    controller = Controller(integrator=integrator, periodicWork=[], statStep=1)

    bounds = (-1, 6, -1, 1.4)
    AnimateScatter(bounds, stepper=controller, positions=pos, frames=10, interval=50)