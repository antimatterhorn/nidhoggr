from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(15)

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
    integrator = RungeKutta4Integrator2d(constantGravity)
  
    print(myNodeList.position())
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    for i in range(30):
        print(integrator.Time(),myNodeList.getFieldVector2d("position")[0].y)
        integrator.Step(0.1)
    