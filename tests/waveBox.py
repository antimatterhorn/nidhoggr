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
        self.meshWriter = SiloMeshWriter2d(baseName="waveBox",nodeList=myNodeList,fieldNames=["phi","xi"])
        self.cycle = dumpCycle
    def __call__(self,cycle,time,dt):
        self.meshWriter.write("-cycle=%03d.silo"%(cycle))
      

if __name__ == "__main__":
    animate = True
    cycles = 20000
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 100
    ny = nx

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=1.0)

    print(waveEqn)

    packages = [waveEqn]

    box = DirichletGridBoundaries2d(grid=grid,physics=waveEqn)
    box.addBox(Vector2d(int(nx/5),int(ny/5)),Vector2d(int(4*nx/5),int(4*ny/5)))
    box.removeBox(Vector2d(int(nx/5)+5,int(ny/5)+5),Vector2d(int(4*nx/5)-5,int(4*ny/5)-5))
    box.removeBox(Vector2d(0,int(ny/2)-5),Vector2d(nx,int(ny/2)+5))
    box.addDomain()
    pbounds = [box]

    integrator = RungeKutta2Integrator2d(packages=packages,
                              dtmin=0.05,
                              boundaries=pbounds)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    periodicWork = [osc]
    if (not animate):
        vtk = vtkdump("testMesh",myNodeList,fieldNames=["phi","xi"],dumpCycle=50)
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
                                                fieldName="phi")
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],frames=cycles)
    else:
        controller.Step(cycles)
