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

class vtkdump:
    def __init__(self,baseName,nodeList,fieldNames,dumpCycle=10):
        self.meshWriter = SiloMeshWriter2d(baseName="testMesh",nodeList=myNodeList,fieldNames=fieldNames)
        self.cycle = dumpCycle
    def __call__(self,cycle,time,dt):
        self.meshWriter.write("%03d.silo"%cycle)

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
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 500
    ny = 500

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveHeight(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,
                             depthMap="sfo.asc")

    print(waveEqn)

    packages = [waveEqn]


    integrator = RungeKutta2Integrator2d(packages=packages,
                              dtmin=0.0001,
                              boundaries=[])
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    periodicWork = [osc]
    vtk = vtkdump("testMesh",myNodeList,fieldNames=["phi","depth"],dumpCycle=100)
    periodicWork.append(vtk)

    controller = Controller(integrator=integrator,
                            statStep=100,
                            periodicWork=periodicWork)

    controller.Step(cycles)
