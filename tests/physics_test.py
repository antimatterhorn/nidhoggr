from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(2)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print("gamma =",eos.gamma)
    #state = State(myNodeList,eos)

    db = DataBase()
    #db.EnrollNodeList(myNodeList,state)

    constantGravity = ConstantGravity2d(myNodeList,constants,Vector2d(0,-9.8))
    integrator = Integrator2d(constantGravity)
  

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)
    print("mass[0] =",myNodeList.mass()[0])
    
    integrator.step(0.1)