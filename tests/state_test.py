from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(2)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    constants = PhysicalConstants(1.0, 1.0, 1.0)
    eos = IdealGasEOS(1.0,constants)

    state = State(myNodeList,eos)

    db = DataBase()
    db.EnrollNodeList(myNodeList,state)

    constantGravity = ConstantGravity2d(db,constants,Vector2d(0,-1))

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)
    print("accel =",myNodeList.velocity())