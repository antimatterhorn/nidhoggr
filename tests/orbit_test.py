from nidhoggr import *

class dumpState:
    def __init__(self,nodeList,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
    def __call__(self):
        self.dump.append((self.nodeList.getFieldVector2d("position")[0].x,self.nodeList.getFieldVector2d("position")[0].y))

if __name__ == "__main__":
    myNodeList = NodeList(1)

    constants = PhysicalConstants(6.378e+6, 5.972e+24, 1.0, 1.0, 1.0) #earth units
    #constants = MKS()



    loc = Vector2d(0, 0)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,
                                      constants=constants,
                                      pointSourceLocation=loc,
                                      pointSourceMass=5)
    integrator = RungeKutta4Integrator2d(physics=sourceGrav,
                                         dtmin=0.1)
  
    pos = myNodeList.getFieldVector2d("position")[0]
    pos.x = -2.0

    velocity = myNodeList.getFieldVector2d("velocity")
    velocity[0].y = -8.686e-4

    dump = dumpState(myNodeList,workCycle=1000)
    periodicWork = [dump]

    controller = Controller(integrator=integrator,periodicWork=periodicWork,statStep=1000)

    print("G =",constants.G)
    controller.Step(100000)
    #myNodeList.getFieldVector2d("position")[0].x,myNodeList.getFieldVector2d("position")[0].y

    import matplotlib.pyplot as plt

    x_values, y_values = zip(*dump.dump)

    plt.plot(x_values, y_values, 'o')  
    plt.plot(loc.x,loc.y,"o",color="red")

    plt.xlabel('x [R_E]')
    plt.ylabel('y [R_E]')
    plt.title('Plot of (x, y)')

    plt.grid(True)
    plt.show()