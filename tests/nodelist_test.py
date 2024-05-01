#!/usr/bin/env python3
from nidhoggr import *

if __name__ == "__main__":
    a = 1
    b = 2

    myField = FieldofDouble("position")
    myField.addValue(a)
    myField.addValue(b)
    #print(myField.name,myField[0])

    myNodeList = NodeList(2)
    myNodeList.addField(myField)
    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)
    print("position[0] =",myNodeList.getField("position")[0])
