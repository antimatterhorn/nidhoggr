from nidhoggr import *

if __name__ == "__main__":
    commandLine = CommandLineArguments(nz = 100,
                                       mass = 1,
                                       dtmin = 0.1e-7,
                                       intVerbose = False)

    myGrid = StellarGrid1d(numZones = nz, totalMass = 1)
    print("grid size:",myGrid.size())
    #print(myGrid.m)
    constants = SOL()

    myNodeList = NodeList(nz)
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    eos = IdealGasEOS(1.4,constants)
    print(eos,"gamma =",eos.gamma)

    #stellar = StellarEvolution1d(myGrid,myNodeList, constants,eos)
    #print("numNodes =",myNodeList.numNodes)
    #print("field names =",myNodeList.fieldNames)
