from nidhoggr import *
from Animation import *
from math import sin,cos

class oscillate:
    def __init__(self,nodeList,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.phi = myNodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        if (time < 200):
            val = 5*(cos(time/10))
            self.phi.setValue(5050,-val)
            self.phi.setValue(2050,val)
            self.phi.setValue(8050,val)
        

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
                                      grid=grid,C=3.0)

    

    print(waveEqn)
    integrator = Integrator2d(physics=waveEqn,
                                         dtmin=0.1)
    print(integrator)


    phi = myNodeList.getFieldDouble("phi")
    print(phi)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(myNodeList,workCycle=1)
    periodicWork = [osc]

    controller = Controller(integrator=integrator,statStep=1000,periodicWork=periodicWork)

    bounds = (nx,ny)
    update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,stepper=controller.Step)
    AnimateGrid2d(bounds,update_method)