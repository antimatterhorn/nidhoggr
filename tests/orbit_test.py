from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(15)

    constants = PhysicalConstants(1.0, 1.0, 1.0, 1.0, 1.0)
    #constants = MKS()



    loc = Vector2d(10, 0)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,constants=constants,pointSourceLocation=loc,pointSourceMass=5)
    integrator = RungeKutta4Integrator2d(sourceGrav)
  
    velocity = myNodeList.getFieldVector2d("velocity")
    velocity[0].y = -5.2e-1

    print(myNodeList.getFieldVector2d("position")[0].x,myNodeList.getFieldVector2d("position")[0].y)

    print("G =",constants.G)
    for i in range(20000):
        if(i%10 == 0):
            print(myNodeList.getFieldVector2d("position")[0].x,myNodeList.getFieldVector2d("position")[0].y)
        integrator.Step(.01)
    