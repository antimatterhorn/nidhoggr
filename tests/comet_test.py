from nidhoggr import *

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
            r = x[i].magnitude
            self.minP = min(self.minP,x[i].magnitude)
        print(self.minP)

class dumpState:
    def __init__(self,nodeList,workCycle=1,G=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
        self.energy = []
        self.G = G
    def __call__(self,cycle,time,dt):
        v2 = self.nodeList.getFieldVector2d("velocity")[0].mag2
        ke = 0.5*v2
        r = self.nodeList.getFieldVector2d("position")[0].magnitude
        pe = self.G/r
        self.dump.append((self.nodeList.getFieldVector2d("position")[0].x,self.nodeList.getFieldVector2d("position")[0].y))
        self.energy.append((time,ke-pe))


if __name__ == "__main__":
    myNodeList = NodeList(2)

    constants = PhysicalConstants(1e30,     # solar mass in  kg
                                  1.5e11,    # au m
                                  3.1e7,          # year in s
                                  1.0, 
                                  1.0) 
    loc = Vector2d(0, 0)
    loc2 = Vector2d(60000,-5000000)
    vs   = Vector2d(0,1)
    cmass = 2.0
    mmass = 1.0
    constants.setG(19.82)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,
                                      constants=constants,
                                      pointSourceLocation=loc,
                                      pointSourceMass=cmass,
                                      pointSourceVelocity = Vector2d(0,0))
    movingGrav = PointSourceGravity2d(nodeList=myNodeList,
                                      constants=constants,
                                      pointSourceLocation=loc2,
                                      pointSourceMass=mmass,
                                      pointSourceVelocity = vs)
    packages = [movingGrav,sourceGrav]
    integrator = RungeKutta4Integrator2d(packages=packages,
                                         dtmin=1e-3)
  
    pos = myNodeList.getFieldVector2d("position")



    norbits = 4

    v0 = 0.019
    velocity = myNodeList.getFieldVector2d("velocity")
    import random
    from math import sqrt

    for i in range(2):
        pos[i].x = random.randint(80000, 120000)
        velocity[i].y = sqrt(cmass*constants.G/10./pos[i].x)

    minPer = minPeri(myNodeList,workCycle=2000,G=constants.G)
    dump = dumpState(myNodeList,workCycle=2000,G=constants.G)
    periodicWork = [minPer,dump]

    import numpy as np

    r0 = 120000
    t0 = 0
    p = v0**2/2 - constants.G*cmass/r0
    a = -constants.G * cmass / (2*p)
    e = np.sqrt(1+2*p*v0**2*r0**2/(constants.G*cmass)**2)

    torbit = 2 * np.pi * np.sqrt(a**3 / (constants.G * cmass))
    print(torbit)

    controller = Controller(integrator=integrator,periodicWork=periodicWork,statStep=10000,tstop=norbits*torbit)

    print("G =",constants.G)
    controller.Step(300000)
    # now plot the orbit
    
    import matplotlib.pyplot as plt

    x_values, y_values = zip(*dump.dump)

    plt.plot(x_values, y_values, 'o',color="green")  
    plt.plot(loc.x,loc.y,"o",color="red")

    def theta(t):
        return t0 + t/r0

    def r(t):
        return a*(1.0-e**2)/(1.0+e*np.cos(theta(t)))


    theta_vec = np.vectorize(theta)
    r_vec = np.vectorize(r)
    # Generate an array of thetas
    t_values = np.linspace(0, 4*np.pi, 100) 

    # Calculate x(t) and y(t) for each time value
    # xs = r_vec(t_values) * np.cos(theta_vec(t_values))
    # ys = r_vec(t_values) * np.sin(theta_vec(t_values))

    # plt.plot(xs,ys)
    time = controller.time
    plt.plot([loc2.x,loc2.x+time*vs.x],[loc2.y,loc2.y+time*vs.y],'b-')

    plt.xlabel('x [AU]')
    plt.ylabel('y [AU]')
    plt.title('Plot of (x, y)')

    plt.grid(True)
    plt.show()

    x_values, y_values = zip(*dump.energy)
    plt.plot(x_values, y_values)
    plt.title('Total Energy')
    plt.xlabel('time [s]')
    plt.ylabel('Specific Energy')
    plt.grid(True)
    plt.show()