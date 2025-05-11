from nidhoggr import *
import numpy as np
import random
from math import sin,cos

class minPeri:
    def __init__(self,nodeList,workCycle=1,G=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.minP = 1e50
        self.energy = []
        self.G = G
    def __call__(self,cycle,time,dt):
        v = self.nodeList.getFieldVector2d("velocity")
        x = self.nodeList.getFieldVector2d("position")
        for i in range(self.nodeList.numNodes):
            self.minP = min(self.minP,x[i].magnitude)
        print(self.minP)

class dumpState:
    def __init__(self,nodeList,workCycle=1,G=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
        self.G = G
    def __call__(self,cycle,time,dt):
        for i in range(self.nodeList.numNodes):
            self.dump.append((self.nodeList.getFieldVector2d("position")[i].x,self.nodeList.getFieldVector2d("position")[i].y))


if __name__ == "__main__":
    commandLine = CommandLineArguments(nComets = 200,
                                       cmass = 1.0,
                                       mmass = 1.0,
                                       norbits = 4,
                                       cycles = 1200000)
    
    myNodeList = NodeList(nComets)

    constants = SOL() # sol units au,msun,yr
    print("G: ",constants.G)

    cmLoc = Vector2d(0,0)
    mmLoc = Vector2d(60000,-500000)
    mmVel = Vector2d(0,0.1)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,
                                      constants=constants,
                                      pointSourceLocation=cmLoc,
                                      pointSourceMass=cmass,
                                      pointSourceVelocity = Vector2d(0,0))
    movingGrav = PointSourceGravity2d(nodeList=myNodeList,
                                      constants=constants,
                                      pointSourceLocation=mmLoc,
                                      pointSourceMass=mmass,
                                      pointSourceVelocity = mmVel)
    packages = [movingGrav,sourceGrav]
    integrator = RungeKutta4Integrator2d(packages=packages,
                                         dtmin=1e-3,verbose=False)

    r0 = 120000
    v0 = np.sqrt(cmass*constants.G/r0)
    t0 = 0
    p = v0**2/2 - constants.G*cmass/r0
    a = -constants.G * cmass / (2*p)
    e = np.sqrt(1+2*p*v0**2*r0**2/(constants.G*cmass)**2)
    torbit = 2 * np.pi * np.sqrt(a**3 / (constants.G * cmass))
    print(torbit)

    pos = myNodeList.getFieldVector2d("position")
    velocity = myNodeList.getFieldVector2d("velocity")

    for i in range(nComets):
        r = float(random.randint(8000,120000))
        t = random.random()*2.0*np.pi
        pos[i].x = r*cos(t)
        pos[i].y = r*sin(t)
        v = 0.8*np.sqrt(cmass*constants.G/r) # 80% of circular velocity
        velocity[i].x = -v*sin(t)
        velocity[i].y = v*cos(t)

    minPer = minPeri(myNodeList,workCycle=10000,G=constants.G)
    dump = dumpState(myNodeList,workCycle=1000,G=constants.G)
    periodicWork = [minPer,dump]

    controller = Controller(integrator=integrator,periodicWork=periodicWork,statStep=10000,tstop=norbits*torbit)

    controller.Step(cycles)
    # now plot the orbit
    
    import matplotlib.pyplot as plt

    x_values, y_values = zip(*dump.dump)

    plt.plot(x_values, y_values, 'o',color="green")  
    plt.plot(cmLoc.x,cmLoc.y,"o",color="red")

    time = controller.time
    plt.plot([mmLoc.x,mmLoc.x+time*mmVel.x],[mmLoc.y,mmLoc.y+time*mmVel.y],'b-')

    plt.xlabel('x [AU]')
    plt.ylabel('y [AU]')
    plt.title('Plot of (x, y)')

    plt.grid(True)
    plt.show()