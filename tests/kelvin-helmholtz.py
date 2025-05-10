from nidhoggr import *
import matplotlib.pyplot as plt
from Animation import *


if __name__ == "__main__":
    animate = True
    
    nx = 100
    ny = 100
    dx = .01
    dy = .01


    myGrid = Grid2d(nx,ny,dx,dy)

    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)

    cycles = 10000
    dtmin = 1e-3

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(1.4,constants)
    print(eos,"gamma =",eos.gamma)


    hydro = GridHydroHLL2d(myNodeList,constants,eos,myGrid)
    
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    box = PeriodicGridBoundaries2d(grid=myGrid)
    # box.addDomain()
    hydro.addBoundary(box)

    integrator = Integrator2d([hydro],dtmin=dtmin)


    density = myNodeList.getFieldDouble("density")
    energy  = myNodeList.getFieldDouble("specificInternalEnergy")
    velocity = myNodeList.getFieldVector2d("velocity")
    position = myNodeList.getFieldVector2d("position")


    for j in range(ny):
        for i in range(nx):
            idx = myGrid.index(i,j,0)
            energy.setValue(idx, 1e-5)
            density.setValue(idx, 1.0)
            pos = position[idx]
            x = pos.x
            y = pos.y
            a = 0.1 * np.sin(4 * np.pi * x / nx)
            if j < ny//2:
                velocity.setValue(idx, Vector2d(-3.0,a))
                density.setValue(idx, 1.0)
            else:
                velocity.setValue(idx, Vector2d(3.0,a))
                density.setValue(idx, 2.0)


    meshWriter = SiloDump(baseName="HLL",
                            nodeList=myNodeList,
                            fieldNames=["density","specificInternalEnergy","pressure"],
                            dumpCycle=200)

    controller = Controller(integrator=integrator,periodicWork=[],statStep=50,tstop=1)

    # controller.Step(cycles)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=hydro.getCell2d,
                                                stepper=controller.Step,
                                                title=title,
                                                fieldName="density")
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],frames=cycles,cmap="plasma")
    else:
        controller.Step(cycles)
