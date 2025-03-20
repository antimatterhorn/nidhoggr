from nidhoggr import *
from Animation import *
from math import sin,cos,sqrt

class oscillate:
    def __init__(self,nodeList,grid,width,height,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.width = width
        self.height = height
        self.phi = myNodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        if time < 0.1:
            a = 2
            x = int(self.width/4)
            y = int(self.height/2)
            for i in range(grid.nx):
                for j in range(grid.ny):
                    idx = self.grid.index(i,j,0)
                    r = sqrt((x-i)**2+(y-j)**2)
                    if r < 1.5:
                        self.phi.setValue(idx,a-0.5*a*r)

class vtkdump:
    def __init__(self,baseName,nodeList,fieldNames,dumpCycle=10):
        self.meshWriter = SiloMeshWriter2d(baseName="waveBox",nodeList=myNodeList,fieldNames=["phi","xi"])
        self.cycle = dumpCycle
    def __call__(self,cycle,time,dt):
        self.meshWriter.write("-cycle=%03d.silo"%(cycle))

class debug:
    def __init__(self,nodeList,debugCycle=1):
        self.nodeList = nodeList
        self.cycle = debugCycle
    def __call__(self,cycle,time,dt):
        phi = self.nodeList.getFieldDouble("phi")
        xi = self.nodeList.getFieldDouble("xi")
        for i in range(self.nodeList.numNodes):
            if phi[i] != 0 or xi[i] != 0:
                print(cycle,i,phi[i],xi[i]) 
      

if __name__ == "__main__":
    animate = True
    cycles = 20000
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 50
    ny = 50

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=1.0)

    print(waveEqn)

    packages = [waveEqn]

    pm = PeriodicGridBoundaries2d(grid=grid)
    waveEqn.addBoundary(pm)

    integrator = RungeKutta4Integrator2d(packages=packages,dtmin=0.01)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    deb = debug(nodeList=myNodeList)
    periodicWork = [osc]

    controller = Controller(integrator=integrator,
                            statStep=1,
                            periodicWork=periodicWork)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,
                                                stepper=controller.Step,
                                                title=title,
                                                fieldName="phi")
        AnimateGrid2d(bounds,update_method,extremis=[-1,1],frames=cycles,cmap="plasma")
    else:
        controller.Step(cycles)
