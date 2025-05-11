from nidhoggr import *

class dumpState:
    def __init__(self,nodeList,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
    def __call__(self,cycle,time,dt):
        y = self.nodeList.getFieldDouble("y")[0]
        self.dump.append([time,y])

if __name__ == "__main__":
    myNodeListEul = NodeList(1)
    myNodeListRK2 = NodeList(1)
    myNodeListRK4 = NodeList(1)

    cycles = 10
    dtmin = 2

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print("gamma =",eos.gamma)


    constantGravity = SimplePhysics2d(myNodeListEul,constants)
    packages = [constantGravity]
    integrator = Integrator2d(packages=packages,dtmin=dtmin,verbose=False)
    dumpEul = dumpState(myNodeListEul,workCycle=1)

    controller = Controller(integrator=integrator,periodicWork=[dumpEul],statStep=10)
    controller.Step(cycles)

    constantGravity = SimplePhysics2d(myNodeListRK2,constants)
    packages = [constantGravity]
    integrator = RungeKutta2Integrator2d(packages=packages,dtmin=dtmin,verbose=False)
    dumpRK2 = dumpState(myNodeListRK2,workCycle=1)

    controller = Controller(integrator=integrator,periodicWork=[dumpRK2],statStep=10)
    controller.Step(cycles)

    constantGravity = SimplePhysics2d(myNodeListRK4,constants)
    packages = [constantGravity]
    integrator = RungeKutta4Integrator2d(packages=packages,dtmin=dtmin,verbose=False)
    dumpRK4 = dumpState(myNodeListRK4,workCycle=1)

    controller = Controller(integrator=integrator,periodicWork=[dumpRK4],statStep=10)
    controller.Step(cycles)
    
    import matplotlib.pyplot as plt
    import numpy as np

    def theta(t):
        return 10.0*t**2 - 0.5*t**3

    theta_vec = np.vectorize(theta)
    t_values = np.linspace(0, cycles*dtmin, cycles*10)

    xe,ye       = zip(*dumpEul.dump)
    xr2,yr2     = zip(*dumpRK2.dump)
    xr4,yr4     = zip(*dumpRK4.dump)

    xs = t_values
    ys = theta_vec(t_values)

    plt.plot(xe, ye, 'o', label="Euler Step")  
    plt.plot(xr2, yr2, 'o', label="RK2")  
    plt.plot(xr4, yr4, 'o', label="RK4")  
    plt.plot(xs,ys,label="analytic")

    plt.xlabel('t')
    plt.ylabel('y')

    plt.grid(True)
    plt.legend()
    plt.show()