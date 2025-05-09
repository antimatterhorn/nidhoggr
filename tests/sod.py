from nidhoggr import *
import matplotlib.pyplot as plt


if __name__ == "__main__":
    animate = False
    
    nx = 500
    ny = 10
    dx = 1
    dy = 1


    myGrid = Grid2d(nx,ny,dx,dy)

    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)

    cycles = 3000
    dtmin = 0.001

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(1.4,constants)
    print(eos,"gamma =",eos.gamma)


    hydro = GridHydroHLL2d(myNodeList,constants,eos,myGrid)
    
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    box = ReflectingGridBoundaries2d(grid=myGrid)
    # box.addDomain()
    hydro.addBoundary(box)

    integrator = Integrator2d([hydro],dtmin=dtmin)


    density = myNodeList.getFieldDouble("density")
    energy  = myNodeList.getFieldDouble("specificInternalEnergy")

    for j in range(ny):
        for i in range(nx):
            idx = myGrid.index(i,j,0)
            if i < nx // 2:
                density.setValue(idx, 1.0)
                energy.setValue(idx, 2.5)   # high pressure side
            else:
                density.setValue(idx, 0.125)
                energy.setValue(idx, 2.0)   # low pressure side

    meshWriter = SiloDump(baseName="HLL",
                            nodeList=myNodeList,
                            fieldNames=["density","specificInternalEnergy","pressure","velocity"],
                            dumpCycle=50)

    controller = Controller(integrator=integrator,periodicWork=[meshWriter],statStep=50)

    controller.Step(cycles)

    xs = []
    ys = []
    position = myNodeList.getFieldVector2d("position")
    for i in range(nx*ny):
        if position[i].y == ((ny/2.0)+(dy/2.0)):
            xs.append(position[i].x)
            ys.append(density[i])
    plt.plot(xs,ys)
    plt.show()
