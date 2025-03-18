from nidhoggr import *
from Animation import *


class dumpState:
    def __init__(self,nodeList,workCycle=1,G=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
        self.energy = []
        self.G = G
    def __call__(self,cycle,time,dt):
        for i in range(self.nodeList.numNodes):
            self.dump.append((self.nodeList.getFieldVector2d("position")[i].x,self.nodeList.getFieldVector2d("position")[i].y))


if __name__ == "__main__":
    from RandomNodeGenerator import RandomNodeGenerator2d
    bounds = [[-1,-1],[1,1]]
    numNodes = 3
    Generator = RandomNodeGenerator2d(numNodes=numNodes,bounds=bounds)
    myNodeList = NodeList(numNodes)

    constants = PhysicalConstants(6.378e+6,     # earth radius in m
                                  5.972e+24,    # earth mass in kg 
                                  1.0,          # s
                                  1.0, 
                                  1.0) 
    loc = Vector2d(0, 0)

    nBodyGrav = NBodyGravity2d(nodeList=myNodeList,
                               constants=constants,
                               plummerLength=0.01)
    packages = [nBodyGrav]

    positions   = myNodeList.getFieldVector2d("position")
    mass        = myNodeList.getFieldDouble("mass")
    for i in range(numNodes):
        mass.setValue(i,1.0)
        positions.setValue(i,Generator.positions[i])

    integrator = RungeKutta4Integrator2d(packages=packages,
                                         dtmin=0.5e1)
  

    dump = dumpState(myNodeList,workCycle=1000,G=constants.G)
    periodicWork = [dump]

    controller = Controller(integrator=integrator,periodicWork=[],statStep=1,tstop=1e3)

    bounds = (-1,1,-1,1)

    AnimateScatter(bounds, stepper=controller, positions=positions, frames=100, interval=50)