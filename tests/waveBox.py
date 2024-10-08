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
    def __init__(self,speaker,nodeList,grid,width,height,workCycle=1):
        self.speaker = speaker
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.width = width
        self.height = height
        self.phi = self.nodeList.getFieldDouble("phi")
    def __call__(self,cycle,time,dt):
        a = self.speaker.get_output(cycle*2.5e-5)*1e3
        #print(cycle*2.5e-3,a)
        i = int(self.width/2)
        j = int(self.height/2)
        idx = self.grid.index(i,j,0)
        self.phi.setValue(idx,a)

from Utilities import Microphone,Speaker,SiloDump 

if __name__ == "__main__":
    animate = False
    dump = False
    cycles = 120000
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

    #osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    spk = Speaker(filename="CantinaBand.wav")
    osc = SpkOscillator(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1,speaker=spk)
    mic = Microphone(nodeList=myNodeList,grid=grid,i=90,j=90,filename='mic.wav')
    periodicWork = [osc,mic]
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
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],frames=cycles)
    else:
        controller.Step(cycles)

    mic.update_wav_header()
