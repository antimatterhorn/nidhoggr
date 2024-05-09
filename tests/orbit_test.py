from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(15)

    constants = PhysicalConstants(6.378e+6, 5.972e+24, 1.0, 1.0, 1.0) #earth units
    #constants = MKS()



    loc = Vector2d(2, 0)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,constants=constants,pointSourceLocation=loc,pointSourceMass=5)
    integrator = RungeKutta4Integrator2d(sourceGrav,dtmin=0.01)
  
    velocity = myNodeList.getFieldVector2d("velocity")
    velocity[0].y = -8.686e-4

    controller = Controller(integrator)

    print("G =",constants.G)
    controller.Step(10)
    