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
        a = 5*(cos(0.5*time))
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

from Utilities import HarmonicOscillator,SiloDump 

if __name__ == "__main__":
    commandLine = CommandLineArguments(animate = True,
                                       dump = False,
                                       cycles = 120000,
                                       nx = 100,
                                       ny = 100,
                                       cs = 1.0,
                                       frequency = 10.0,
                                       amplitude  = 8.0,
                                       ns = 5)
    
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
    # --- 1. Left vertical bar ---
    x0 = int(nx * 0.1)
    x1 = int(nx * 0.2)
    box.addBox(Vector2d(x0, 0.1*ny), Vector2d(x1, 0.9*ny))

    # --- 2. Right vertical bar ---
    x2 = int(nx * 0.8)
    x3 = int(nx * 0.9)
    box.addBox(Vector2d(x2, 0.1*ny), Vector2d(x3, 0.9*ny))

    # --- 3. Diagonal bar approximation ---
    # We'll make thin boxes that step diagonally upward
    num_steps = int(0.8*ny)
    for step in range(num_steps):
        y0 = step
        y1 = step + 1
        # Interpolate x from left to right as y increases
        x = int(x2 - (x2 - x0) * (step / num_steps))
        box.addBox(Vector2d(x, y0+0.1*ny), Vector2d(x + 8, y1+0.1*ny))
    box.addDomain()

    waveEqn.addBoundary(box)

    integrator = RungeKutta4Integrator2d(packages=packages,dtmin=0.05)
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    spk = HarmonicOscillator(frequency=frequency,amplitude=amplitude)
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
                                                fieldName="phi")
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],frames=cycles,cmap="plasma")
    else:
        controller.Step(cycles)
