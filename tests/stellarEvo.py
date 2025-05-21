from nidhoggr import *

if __name__ == "__main__":
    commandLine = CommandLineArguments(nz = 100,
                                       mass = 1,
                                       dtmin = 0.1e-7,
                                       intVerbose = False)
    constants = SOL()

    myNodeList = NodeList(nz)
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    eos = IdealGasEOS(5.0/3.0,constants)
    print(eos,"gamma =",eos.gamma)

    stellar = StellarEvolution(myNodeList, constants, eos, mass, 15e6)
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    integrator = CrankNicolsonIntegrator1d(packages=[stellar],dtmin=dtmin)

    controller = Controller(integrator=integrator,periodicWork=[],statStep=1)

    #controller.Step(2)
