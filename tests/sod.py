from nidhoggr import *
import matplotlib.pyplot as plt
from Animation import *

if __name__ == "__main__":
    commandLine = CommandLineArguments(animate = True,
                                       siloDump = False,
                                        cycles = 3000,
                                        nx = 100,
                                        ny = 20,
                                        dx = 1,
                                        dy = 1,
                                        dtmin = 0.001)

    myGrid = Grid2d(nx,ny,dx,dy)
    print("grid size:",myGrid.size())
    
    myNodeList = NodeList(nx*ny)
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
    hydro.addBoundary(box)

    integrator = RungeKutta4Integrator2d([hydro],dtmin=dtmin)

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

    periodicWork = []
    
    if siloDump:
        meshWriter = SiloDump(baseName="HLL",
                                nodeList=myNodeList,
                                fieldNames=["density","specificInternalEnergy","pressure","velocity"],
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
        AnimateGrid2d(bounds,update_method,extremis=[-5,5],frames=cycles,cmap="plasma")
    else:
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
