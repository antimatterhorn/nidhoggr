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
    myNodeList = NodeList(1)

    cycles = 10
    dtmin = 2

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print("gamma =",eos.gamma)


    constantGravity = SimplePhysics2d(myNodeList,constants)
    packages = [constantGravity]
    integrator = RungeKutta4Integrator2d(packages=packages,dtmin=dtmin)

    print(myNodeList.position())
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    dump = dumpState(myNodeList,workCycle=1)

    controller = Controller(integrator=integrator,periodicWork=[dump],statStep=1)
    controller.Step(cycles)
    
    import matplotlib.pyplot as plt
    import numpy as np

    def theta(t):
        return 10.0*t**2 - 0.5*t**3

    theta_vec = np.vectorize(theta)
    t_values = np.linspace(0, cycles*dtmin, cycles*10)

    x_values, y_values = zip(*dump.dump)

    xs = t_values
    ys = theta_vec(t_values)

    plt.plot(x_values, y_values, 'o', label="simulated")  
    plt.plot(xs,ys,label="analytic")

    plt.xlabel('t')
    plt.ylabel('y')

    plt.grid(True)
    plt.legend()
    plt.show()