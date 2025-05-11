from nidhoggr import *
import matplotlib.pyplot as plt
from Animation import *


if __name__ == "__main__":
    commandLine = CommandLineArguments(animate = True,
                                        siloDump = False,
                                        cycles = 3000,
                                        nx = 100,
                                        ny = 100,
                                        dx = 1,
                                        dy = 1,
                                        dtmin = 0.001,
                                        intVerbose = False)

    myGrid = Grid2d(nx,ny,dx,dy)
    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    constants = MKS()
    eos = IdealGasEOS(1.4,constants)
    print(eos,"gamma =",eos.gamma)

    hydro = GridHydroHLL2d(myNodeList,constants,eos,myGrid)

    box = ReflectingGridBoundaries2d(grid=myGrid)
    hydro.addBoundary(box)

    gravityVector = Vector2d(0.,-10)
    gravity  = ConstantGridAccel2d(myNodeList,constants,gravityVector)

    integrator = RungeKutta4Integrator2d([hydro,gravity],dtmin=dtmin,verbose=intVerbose)

    density = myNodeList.getFieldDouble("density")
    energy  = myNodeList.getFieldDouble("specificInternalEnergy")
    velocity = myNodeList.getFieldVector2d("velocity")
    position = myNodeList.getFieldVector2d("position")

    p0 = 2.5
    gamma = eos.gamma

    for j in range(ny):
        for i in range(nx):
            idx = myGrid.index(i,j,0)
            energy.setValue(idx, 1.0)
            density.setValue(idx, 1.0)
            pos = position[idx]
            x = pos.x
            y = pos.y

            # sinusoidal vertical offset of the interface
            interface_y = ny / 2 + 0.8 * np.sin(5.0 * np.pi * x / nx)

            if y < interface_y:
                rho = 1.0
                density.setValue(idx, rho)  # heavy fluid below
                energy.setValue(idx, p0 / ((gamma - 1.0) * rho))
            else:
                rho = 2.0
                density.setValue(idx, rho)  # light fluid above
                energy.setValue(idx, p0 / ((gamma - 1.0) * rho))

    periodicWork = []

    if siloDump:
        meshWriter = SiloDump(baseName="HLL",
                                nodeList=myNodeList,
                                fieldNames=["density","specificInternalEnergy","pressure","velocity","acceleration"],
                                dumpCycle=50)
        periodicWork += [meshWriter]

    controller = Controller(integrator=integrator,periodicWork=periodicWork,statStep=50)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (nx,ny)
        update_method = AnimationUpdateMethod2d(call=hydro.getCell2d,
                                                stepper=controller.Step,
                                                title=title,
                                                fieldName="density")
        AnimateGrid2d(bounds,update_method,extremis=[0,5],frames=cycles,cmap="plasma")
    else:
        controller.Step(cycles)
