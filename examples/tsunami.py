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
        if (time<(3.14159/12)):
            a = -40*(cos(time*6))
            i = int(2*self.width/5)
            j = int(self.height/2)
            idx = self.grid.index(i,j,0)
            self.phi.setValue(idx,a)

from Utilities import SiloDump

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
    cycles = 10000

    nx = 722
    ny = 696
    yllcorner = 36.3
    cellsize = 0.004166666667
    earthRadius = 6371000.0;
    cellsize_meters = 2 * 3.14159 * earthRadius * cellsize * cos(yllcorner * 3.14159 / 180.0) / 360.0;

    print("cell size: %3.3em\n"%cellsize_meters)

    constants = MKS()

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,cellsize_meters,cellsize_meters)
    print(grid)

    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,
                             depthMap="sfo.asc")

    print(waveEqn)
    packages = [waveEqn]

    integrator = RungeKutta2Integrator2d(packages=packages,
                              dtmin=0.0001,verbose=False)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    periodicWork = [osc]
    vtk = SiloDump("testMesh",myNodeList,fieldNames=["phi","depth","maxphi"],dumpCycle=100)
    periodicWork.append(vtk)

    controller = Controller(integrator=integrator,
                            statStep=50,
                            periodicWork=periodicWork)

    controller.Step(cycles)
