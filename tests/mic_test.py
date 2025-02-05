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
        a = 5.0*(cos(0.5*time))
        self.phi.setValue(0,a)

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
        #print(cycle*1e-4,a)
        self.phi.setValue(0,a)

from Utilities import Microphone,Speaker

if __name__ == "__main__":
    # Some problem setup conditions
    cycles = 60000
    constants = PhysicalConstants(1,1,1.0,1.0,1.0) 
    nx = 1
    ny = nx

    # Create the nodelist and grid
    myNodeList = NodeList(nx*ny)
    grid = Grid2d(nx,ny,1,1)
    print("grid %dx%d"%(nx,ny))
    print(grid)

    # Create the physics package and add to physics packages
    waveEqn = WaveEquation2d(nodeList=myNodeList,
                             constants=constants,
                             grid=grid,C=1.0)
    print(waveEqn)
    packages = [waveEqn]

    # Create the integrator and assign packages to it
    integrator = RungeKutta2Integrator2d(packages=packages,
                              dtmin=0.05,
                              boundaries=[])
    print(integrator)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    # IO for periodic work
    #osc = oscillate(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1)
    spk = Speaker(filename="CantinaBand.wav")
    osc = SpkOscillator(nodeList=myNodeList,grid=grid,width=nx,height=ny,workCycle=1,speaker=spk)
    mic = Microphone(nodeList=myNodeList,grid=grid,i=0,j=0,filename='mic.wav')
    periodicWork = [osc,mic]

    # Create controller object and assign periodic work
    controller = Controller(integrator=integrator,
                            statStep=100,
                            periodicWork=periodicWork)

    # Step
    controller.Step(cycles)
    mic.update_wav_header()
