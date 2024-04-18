from nidhoggr import *

if __name__ == "__main__":
    a = 1
    b = 2

    myFieldList = FieldListofDouble("position",2)
    myFieldList.addField(a)
    myFieldList.addField(b)
    print(myFieldList.name)

    myNodeList = NodeList(2)
    #myNodeList.addFieldListofDouble(myFieldList)
    print(myNodeList.numNodes)
    print(myNodeList.fieldNames)
