from nidhoggr import *
from Animation import *
from math import sin,cos

class oscillate:
    def __init__(self,nodeList,grid,width,height,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.width = width
        self.height = height
        self.phi = myNodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        a = 5*(cos(time))
        i = int(self.width/6)
        j = int(self.height/4)
        idx = self.grid.index(i,j,0)
        self.phi.setValue(idx,a)
        a = 5*(cos(time*1.1))
        i = int(self.width/6)
        j = int(self.height*3/4)
        idx = self.grid.index(i,j,0)
        self.phi.setValue(idx,a)

from Utilities import SiloDump
      

if __name__ == "__main__":
    commandLine = CommandLineArguments(animate = True,
                                       cycles = 20000,
                                       nx = 100,
                                       ny = 100)
    

    constants = MKS()

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=1.0)

    print(waveEqn)

    packages = [waveEqn]

    pm = OutflowGridBoundaries2d(grid=grid)
    box = DirichletGridBoundaries2d(grid=grid)

    nbox = 10
    dy = ny/nbox
    h  = int(dy/2.5)
    for i in range(nbox):
        x = int(nx/3)
        y = dy*(i+1)
        box.addSphere(Vector2d(x,y),2)

    waveEqn.addBoundary(pm)
    waveEqn.addBoundary(box)

    integrator = RungeKutta4Integrator2d(packages=packages,
                              dtmin=0.05)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    periodicWork = [osc]
    if (not animate):
        vtk = SiloDump("testMesh",myNodeList,fieldNames=["phi","xi"],dumpCycle=50)
        periodicWork.append(vtk)

    controller = Controller(integrator=integrator,
                            statStep=100,
                            periodicWork=periodicWork)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,
                                                stepper=controller.Step,
                                                title=title,
                                                fieldName="maxphi")
        AnimateGrid2d(bounds,update_method,extremis=[0,2],frames=cycles,cmap='plasma')
    else:
        controller.Step(cycles)
