from nidhoggr import *
from Animation import *
from math import sin,cos

class oscillate:
    def __init__(self,nodeList,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.phi = myNodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        val = 5*(cos(time))
        self.phi.setValue(5050,-val)
        self.phi.setValue(2025,val)
        self.phi.setValue(8075,val)

      

if __name__ == "__main__":
    animate = True
    
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 100
    ny = 100

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=1.0)

    print(waveEqn)

    pm = PacmanGridBoundaries2d(grid=grid,physics=waveEqn)
    print(pm)
    box = DirichletGridBoundaries2d(grid=grid,physics=waveEqn)
    box.addBox(Vector2d(5,5),Vector2d(80,15))
    box.addBox(Vector2d(80,30),Vector2d(95,60))
    box.removeBox(Vector2d(40,0),Vector2d(60,15))
    pbounds = [pm,box]

    integrator = RungeKutta2Integrator2d(physics=waveEqn,
                              dtmin=0.05,
                              boundaries=pbounds)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(myNodeList,workCycle=1)
    periodicWork = [osc]

    controller = Controller(integrator=integrator,
                            statStep=10,
                            periodicWork=periodicWork)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,
                                                stepper=controller.Step,
                                                title=title)
        AnimateGrid2d(bounds,update_method,extremis=[-5,5])
    else:
        controller.Step(50000)
