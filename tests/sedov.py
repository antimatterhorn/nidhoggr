from nidhoggr import *
import matplotlib.pyplot as plt


if __name__ == "__main__":
    animate = False
    
    nx = 100
    ny = 100
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

    # Parameters
    x0 = nx // 2
    y0 = ny // 2
    sigma = 1.5  # width in grid units
    e0 = 1000.0  # total peak energy density

    for j in range(ny):
        for i in range(nx):
            idx = myGrid.index(i, j, 0)
            
            # Distance from center
            ddx = i - x0
            ddy = j - y0
            r2 = ddx*ddx + ddy*ddy
            
            # Gaussian profile
            e = e0 * np.exp(-r2 / (2.0 * sigma**2))
            
            energy.setValue(idx, e)
            density.setValue(idx, 1.0)


    meshWriter = SiloDump(baseName="HLL",
                            nodeList=myNodeList,
                            fieldNames=["density","specificInternalEnergy","pressure"],
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
