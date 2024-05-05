from nidhoggr import *

if __name__ == "__main__":
    myNodeList = NodeList(2)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    constantGravity = constantGravity2d()