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
        self.phi = self.nodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        a = 5*(cos(0.5*time))*np.exp(-time)
        i = int(self.width/2)
        j = int(self.height/2)
        idx = self.grid.index(i,j,0)
        self.phi.setValue(idx,a)

class SpkOscillator:
    def __init__(self,speaker,nodeList,grid,width,height,ns,workCycle=1):
        self.speaker = speaker
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.width = width
        self.height = height
        self.phi = self.nodeList.getFieldDouble("phi")
        self.ns = ns
    def __call__(self,cycle,time,dt):
        m = self.speaker.get_output(cycle*2.5e-3)
        #print(cycle*2.5e-3,a)
        wa = self.width/self.ns
        wb = self.height/self.ns
        for a in range(self.ns):
            for b in range(self.ns):
                j = int(wa+a*wa)
                i = int(wb+b*wb)
                idx = self.grid.index(j,i,0)
                self.phi.setValue(idx,m)

from Utilities import DampedHarmonicOscillator,SiloDump 

if __name__ == "__main__":
    commandLine = CommandLineArguments(animate = True,
                                       dump = False,
                                       cycles = 120000,
                                       nx = 150,
                                       ny = 80,
                                       cs = 1.0,
                                       frequency = 10.0,
                                       amplitude  = 8.0,
                                       damping = -5.0,
                                       ns = 16)
    
    constants = MKS()

    myNodeList = NodeList(nx*ny)
    
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=cs)

    print(waveEqn)

    packages = [waveEqn]

    box = DirichletGridBoundaries2d(grid=grid)
    box.addBox(Vector2d(16,20),Vector2d(20,44))
    box.addBox(Vector2d(16,20),Vector2d(96,24))
    box.addBox(Vector2d(20,44),Vector2d(52,48))
    box.addBox(Vector2d(24,20),Vector2d(28,32))
    box.addBox(Vector2d(32,20),Vector2d(36,56))
    box.addBox(Vector2d(36,56),Vector2d(40,60))
    box.addBox(Vector2d(40,20),Vector2d(44,56))
    box.addBox(Vector2d(48,44),Vector2d(52,56))
    box.addBox(Vector2d(48,32),Vector2d(52,40))
    box.addBox(Vector2d(52,56),Vector2d(56,60))
    box.addBox(Vector2d(56,20),Vector2d(60,56))
    box.addBox(Vector2d(60,56),Vector2d(64,60))
    box.addBox(Vector2d(64,44),Vector2d(68,56))
    box.addBox(Vector2d(64,44),Vector2d(72,48))
    box.addBox(Vector2d(64,20),Vector2d(68,32))
    box.addBox(Vector2d(72,20),Vector2d(76,44))
    box.addBox(Vector2d(76,44),Vector2d(88,48))
    box.addBox(Vector2d(80,32),Vector2d(84,40))
    box.addBox(Vector2d(88,8),Vector2d(92,44))
    box.addBox(Vector2d(92,44),Vector2d(104,48))
    box.addBox(Vector2d(108,44),Vector2d(120,48))
    box.addBox(Vector2d(124,44),Vector2d(136,48))
    box.addBox(Vector2d(104,8),Vector2d(108,44))
    box.addBox(Vector2d(120,8),Vector2d(124,44))
    box.addBox(Vector2d(88,8),Vector2d(124,12))
    box.addBox(Vector2d(104,20),Vector2d(112,24))
    box.addBox(Vector2d(96,32),Vector2d(100,40))
    box.addBox(Vector2d(112,32),Vector2d(116,40))
    box.addBox(Vector2d(120,20),Vector2d(132,24))
    box.addBox(Vector2d(128,20),Vector2d(132,32))
    box.addBox(Vector2d(132,32),Vector2d(140,36))
    box.addBox(Vector2d(136,32),Vector2d(140,44))


    box.addDomain()

    waveEqn.addBoundary(box)

    integrator = RungeKutta4Integrator2d(packages=packages,dtmin=0.05)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    spk = DampedHarmonicOscillator(frequency=frequency,amplitude=amplitude,damping=damping)
    osc = SpkOscillator(nodeList=myNodeList,grid=grid,width=nx,height=ny,ns=ns,workCycle=1,speaker=spk)
    periodicWork = [osc]
    if(dump):
        silo = SiloDump("testMesh",myNodeList,fieldNames=["phi","xi"],dumpCycle=50)
        periodicWork.append(silo)

    controller = Controller(integrator=integrator,
                            statStep=500,
                            periodicWork=periodicWork)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=waveEqn.getCell2d,
                                                stepper=controller.Step,
                                                title=title,
                                                fieldName="maxphi")
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],frames=cycles,cmap="plasma")
    else:
        controller.Step(cycles)
