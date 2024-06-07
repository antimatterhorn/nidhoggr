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
        i = int(self.width/2)
        j = int(self.height/2)
        idx = self.grid.index(i,j,0)
        self.phi.setValue(idx,a)

class vtkdump:
    def __init__(self,baseName,nodeList,fieldNames,dumpCycle=10):
        self.meshWriter = SiloMeshWriter2d(baseName="testMesh",nodeList=myNodeList,fieldNames=["phi","depth"])
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
    animate = False
    cycles = 2
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 20
    ny = 20

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveHeight(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,
                             depthMap="output.asc")

    print(waveEqn)

    packages = [waveEqn]


    integrator = Integrator2d(packages=packages,
                              dtmin=0.05,
                              boundaries=[])
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    periodicWork = [osc]
    if (not animate):
        vtk = vtkdump("testMesh",myNodeList,fieldNames=["phi","xi"],dumpCycle=10)
        periodicWork.append(vtk)

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
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],frames=cycles)
    else:
        controller.Step(cycles)
