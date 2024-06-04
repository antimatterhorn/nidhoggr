from nidhoggr import *
from Animation import *
from math import sin,cos

class oscillate:
    def __init__(self,nodeList,grid,depth,width,height,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.depth = depth
        self.width = width
        self.height = height
        self.phi = myNodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        a = 5*(cos(time))
        i = int(self.width/2)
        j = int(self.height/2)
        k = int(self.depth/2)        
        idx = self.grid.index(i,j,k)
        print(self.phi[idx])
        self.phi.setValue(idx,a)
        print("setting %d to %f"%(idx,a))

      

if __name__ == "__main__":
    
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 100
    ny = 100
    nz = 100
    cycles = 30

    myNodeList = NodeList(nx*ny*nz)
    
    grid = Grid3d(nx,ny,nz,1,1,1)
    print("grid %dx%dx%d"%(nx,ny,nz))
    print(grid)

    waveEqn = WaveEquation3d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=1.0)

    print(waveEqn)

    packages = [waveEqn]

    pm = PacmanGridBoundaries3d(grid=grid,physics=waveEqn)
    print(pm)
    pbounds = [pm]

    integrator = RungeKutta2Integrator3d(packages=packages,
                              dtmin=0.05,
                              boundaries=pbounds)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,depth=nz,workCycle=1)
    periodicWork = [osc]

    controller = Controller(integrator=integrator,
                            statStep=10,
                            periodicWork=periodicWork)

    controller.Step(cycles)
    meshWriter = VTKMeshWriter3d(baseName="testMesh",nodeList=myNodeList,fieldNames=["phi","xi"])
    meshWriter.write("00.vtk")
