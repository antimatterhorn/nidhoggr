from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(15)

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    print("G =",constants.G)
    eos = IdealGasEOS(5.0/3.0,constants)
    print("gamma =",eos.gamma)
    #state = State(myNodeList,eos)

    db = DataBase()
    #db.EnrollNodeList(myNodeList,state)

    loc = Vector2d(6.37e6*2,0)

    sourceGrav = PointSourceGravity2d(nodeList=myNodeList,constants=constants,pointSourceLocation=loc,pointSourceMass=5.97e24)
    integrator = RungeKutta4Integrator2d(sourceGrav)
  
    velocity = myNodeList.getFieldVector2d("velocity")
    velocity[0].y = -5590


    for i in range(500):
        print(myNodeList.getFieldVector2d("position")[0].x,myNodeList.getFieldVector2d("position")[0].y)
        integrator.Step(1e6)
    