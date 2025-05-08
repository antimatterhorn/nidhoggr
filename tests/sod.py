from nidhoggr import *

if __name__ == "__main__":
    animate = False
    
    nx = 500
    ny = 50
    dx = 1
    dy = 1


    myGrid = Grid2d(nx,ny,dx,dy)

    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)

    cycles = 2000
    dtmin = 0.01

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print(eos,"gamma =",eos.gamma)


    hydro = GridHydroHLL2d(myNodeList,constants,eos,myGrid)
    
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    # box = DirichletGridBoundaries2d(grid=myGrid)
    # box.addDomain()
    # hydro.addBoundary(box)

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

    meshWriter = SiloDump(baseName="HLL",nodeList=myNodeList,fieldNames=["density","specificInternalEnergy","pressure"])

    controller = Controller(integrator=integrator,periodicWork=[meshWriter],statStep=50)

    controller.Step(cycles)
