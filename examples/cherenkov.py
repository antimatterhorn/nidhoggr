from nidhoggr import *
from Animation import *
from math import sin,cos

class oscillate:
    def __init__(self,nodeList,grid,cs,width,height,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.cs = cs
        self.width = width
        self.height = height
        self.phi = myNodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        a = 5*(cos(time/0.5))
        v = min(max((0.02*time+0.5)*self.cs,0.5*self.cs),2.0*self.cs)
        posx = v*time
        if posx <= self.width:
            i = int(posx)
            j = int(self.height/2)
            idx = self.grid.index(i,j,0)
            self.phi.setValue(idx,a)
        # self.phi.setValue(5050,-val)
        # self.phi.setValue(2025,val)
        # self.phi.setValue(8075,val)

from Utilities import SiloDump

if __name__ == "__main__":
    # ------------------------------------------------
    # Some problem setup conditions
    # ------------------------------------------------
    animate = True
    constants = MKS()
    nx = 100
    ny = 100
    cs = 1.0

    # ------------------------------------------------
    # Create the nodelist and grid
    # ------------------------------------------------
    myNodeList = NodeList(nx*ny)
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    # ------------------------------------------------
    # Create the physics package and add to physics packages
    # ------------------------------------------------
    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=cs)
    print(waveEqn)
    packages = [waveEqn]

    # Create boundaries
    pm = PeriodicGridBoundaries2d(grid=grid,physics=waveEqn)
    pbounds = []

    # ------------------------------------------------
    # Create the integrator and assign packages to it
    # ------------------------------------------------
    integrator = RungeKutta4Integrator2d(packages=packages,
                              dtmin=0.01,
                              boundaries=pbounds)
    print(integrator)
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    # ------------------------------------------------
    # IO for periodic work
    # ------------------------------------------------
    osc = oscillate(nodeList=myNodeList,grid=grid,cs=cs,width=nx,height=ny,workCycle=1)
    vtk = SiloDump("testMesh",myNodeList,fieldNames=["phi","xi"],dumpCycle=100)
    if (animate):
        periodicWork = [osc]
    else:
        periodicWork = [osc,vtk]
    
    # ------------------------------------------------
    # Create controller object and assign periodic work
    # ------------------------------------------------
    controller = Controller(integrator=integrator,
                            statStep=10,
                            periodicWork=periodicWork)

    # ------------------------------------------------
    # Step
    # ------------------------------------------------
    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,
                                                stepper=controller.Step,
                                                title=title,
                                                fieldName="phi")
        AnimateGrid2d(bounds,update_method,extremis=[-1,1],cmap="plasma",frames=1000)
    else:
        controller.Step(5000)
