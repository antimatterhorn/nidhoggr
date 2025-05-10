from nidhoggr import *
import matplotlib.pyplot as plt
from Animation import *


if __name__ == "__main__":
    animate = True
    
    nx = 100
    ny = 100
    dx = 1
    dy = 1


    myGrid = Grid2d(nx,ny,dx,dy)

    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)

    cycles = 10000
    dtmin = 1e-5

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

    gravityVector = Vector2d(0.,-20)
    gravity  = ConstantGridAccel2d(myNodeList,constants,gravityVector)

    integrator = RungeKutta4Integrator2d([hydro,gravity],dtmin=dtmin)


    density = myNodeList.getFieldDouble("density")
    energy  = myNodeList.getFieldDouble("specificInternalEnergy")
    velocity = myNodeList.getFieldVector2d("velocity")
    position = myNodeList.getFieldVector2d("position")


    for j in range(ny):
        for i in range(nx):
            idx = myGrid.index(i,j,0)
            energy.setValue(idx, 1.0)
            density.setValue(idx, 1.0)
            pos = position[idx]
            x = pos.x
            y = pos.y

            # sinusoidal vertical offset of the interface
            interface_y = ny / 2 + 1.0 * np.sin(5.0 * np.pi * x / nx)

            if y < interface_y:
                density.setValue(idx, 2.0)  # heavy fluid below
            else:
                density.setValue(idx, 1.0)  # light fluid above


    meshWriter = SiloDump(baseName="HLL",
                            nodeList=myNodeList,
                            fieldNames=["density","specificInternalEnergy","pressure"],
                            dumpCycle=200)

    controller = Controller(integrator=integrator,periodicWork=[],statStep=1,tstop=1)

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
