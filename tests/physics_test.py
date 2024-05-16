from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(15)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print("gamma =",eos.gamma)


    constantGravity = ConstantGravity2d(myNodeList,constants,Vector2d(0,-9.8))
    integrator = RungeKutta4Integrator2d(constantGravity,dtmin=1e-3)
  
    velocity = myNodeList.getFieldVector2d("velocity")
    velocity[0].y = 10.0

    print(myNodeList.position())
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    controller = Controller(integrator=integrator,periodicWork=[],statStep=1)
    controller.Step(30)
    