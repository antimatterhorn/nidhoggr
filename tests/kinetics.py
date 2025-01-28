from nidhoggr import *
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import random
from Animation import *

if __name__ == "__main__":
    animate = True
    
    numNodes = 2

    constants = MKS()
    myNodeList = NodeList(numNodes)

    gravVec = Vector2d(0, 0)

    constantGravity = ConstantGravity2d(myNodeList, constants, gravVec)
    kinetics = Kinetics2d(myNodeList,constants)
    packages = [constantGravity,kinetics]

    integrator = RungeKutta2Integrator2d(packages=packages, dtmin=0.01, boundaries=[])
    print(integrator)


    rad = myNodeList.getFieldDouble("radius")
    mass = myNodeList.getFieldDouble("mass")
    for i in range(numNodes):
        rad.setValue(i,2)
        mass.setValue(i,0.2)

    print("numNodes =", myNodeList.numNodes)
    print("field names =", myNodeList.fieldNames)

    pos = myNodeList.getFieldVector2d("position")
    vel = myNodeList.getFieldVector2d("velocity")
    for i in range(numNodes):
        pos.setValue(i, Vector2d(-10+i/numNodes*30, -0.5+i/numNodes*1))
        vel.setValue(i, Vector2d(3*(-1)**i,0))

    controller = Controller(integrator=integrator, periodicWork=[], statStep=1)

    bounds = (-10, 10, -10, 10)
    ani = AnimateScatter(positions=pos,stepper=controller.Step)
    AnimateScatter(bounds, update_method=ani, frames=5, interval=50)
