from nidhoggr import *
from Animation import *

class dumpState:
    def __init__(self,nodeList,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
    def __call__(self):
        self.dump.append((self.nodeList.getFieldVector2d("position")[0].x,self.nodeList.getFieldVector2d("position")[0].y))

if __name__ == "__main__":
    myNodeList = NodeList(100*100)

    constants = PhysicalConstants(6.378e+6,     # earth mass in kg
                                  5.972e+24,    # earth radius in m
                                  1.0,          # s
                                  1.0, 
                                  1.0) 
    nx = 100
    ny = 100
    
    grid = Grid2d(nx,ny,1,1)





    print("grid %dx%d"%(nx,ny))
    print(grid)
    waveEqn = WaveEquation2d(nodeList=myNodeList,
                                      constants=constants,
                                      grid=grid)

    

    print(waveEqn)
    integrator = Integrator2d(physics=waveEqn,
                                         dtmin=0.1)
    print(integrator)


    phi = myNodeList.getFieldDouble("phi")
    print(phi)
    phi.setValue(5050,10)
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    # dump = dumpState(myNodeList,workCycle=1000)
    # periodicWork = [dump]

    controller = Controller(integrator=integrator,statStep=1000)
    controller.Step()

    bounds = (nx,ny)
    update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,stepper=controller.Step)
    AnimateGrid2d(bounds,update_method)