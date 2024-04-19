#!/usr/bin/env python3
from nidhoggr import *

if __name__ == "__main__":
    a = 1
    b = 2

    myFieldList = FieldListofDouble("position")
    myFieldList.addField(a)
    myFieldList.addField(b)
    print(myFieldList.name,myFieldList[0])

    myNodeList = NodeList(2)
    #myNodeList.addFieldListofDouble(myFieldList)
    #print(myNodeList.numNodes)
    #print(myNodeList.fieldNames)
