from PYB11Generator import *

class NodeList:
    def pyinit(self):
        return
    def pyinit1(self,numNodes="int"):
        return
    def addFieldList(self,fieldList="FieldListBase*"):
        return
    
    numNodes = PYB11property("int", getter="getNumNodes", doc="The number of nodes in the nodeList.")
    count = PYB11property("int", getter="getFieldListCount", doc="The number of fieldLists in the nodeList.")