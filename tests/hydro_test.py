from nidhoggr import *

# class dumpState:
#     def __init__(self,nodeList,workCycle=1):
#         self.nodeList = nodeList
#         self.cycle = workCycle
#         self.dump = []
#     def __call__(self,cycle,time,dt):
#         y = self.nodeList.getFieldDouble("y")[0]
#         self.dump.append([time,y])

if __name__ == "__main__":
    nx = 10
    ny = 10
    dx = 0.1
    dy = 0.1


    myGrid = Grid2d(nx,ny,dx,dy)

    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)

    cycles = 10
    dtmin = 2

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print(eos,"gamma =",eos.gamma)


    hydro = HydroHLL2d(myNodeList,constants,eos,myGrid)
    
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)
    
    integrator = Integrator2d(hydro,dtmin=dtmin)


    density = myNodeList.getFieldDouble("density")
    energy  = myNodeList.getFieldDouble("specificInternalEnergy")
    for i in range (nx*ny):
        density.setValue(i,1.0)
        energy.setValue(i,1.0)

    # dump = dumpState(myNodeList,workCycle=1)

    controller = Controller(integrator=integrator,periodicWork=[],statStep=1)
    controller.Step(cycles)
    
    # import matplotlib.pyplot as plt
    # import numpy as np

    # def theta(t):
    #     return t**2

    # theta_vec = np.vectorize(theta)
    # t_values = np.linspace(0, cycles*dtmin, cycles*10)

    # x_values, y_values = zip(*dump.dump)

    # xs = t_values
    # ys = theta_vec(t_values)

    # plt.plot(x_values, y_values, 'o')  
    # plt.plot(xs,ys)

    # plt.xlabel('t')
    # plt.ylabel('y')

    # plt.grid(True)
    # plt.show()