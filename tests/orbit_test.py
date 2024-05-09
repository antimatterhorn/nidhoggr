from nidhoggr import *

class dumpState:
    def __init__(self,nodeList,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
    def __call__(self):
        print(self.nodeList.getFieldVector2d("position")[0].x,self.nodeList.getFieldVector2d("position")[0].y)

if __name__ == "__main__":
    myNodeList = NodeList(15)

    constants = PhysicalConstants(6.378e+6, 5.972e+24, 1.0, 1.0, 1.0) #earth units
    #constants = MKS()



    loc = Vector2d(2, 0)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,
                                      constants=constants,
                                      pointSourceLocation=loc,
                                      pointSourceMass=5)
    integrator = RungeKutta4Integrator2d(physics=sourceGrav,
                                         dtmin=0.01)
  
    velocity = myNodeList.getFieldVector2d("velocity")
    velocity[0].y = -8.686e-4

    dump = dumpState(myNodeList,workCycle=10)

    periodicWork = [dump]

    controller = Controller(integrator=integrator,periodicWork=periodicWork,statStep=100)

    print("G =",constants.G)
    controller.Step(100)
    #myNodeList.getFieldVector2d("position")[0].x,myNodeList.getFieldVector2d("position")[0].y