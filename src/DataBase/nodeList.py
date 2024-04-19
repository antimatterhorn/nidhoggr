from PYB11Generator import *

class NodeList:
    def pyinit(self):
        return
    def pyinit1(self,numNodes= ("unsigned", "0")):
        return
    @PYB11pycppname("addFieldList")
    def addFieldListofDouble(self,fieldList="FieldListofDouble"):
        return
    @PYB11pycppname("addFieldList")
    def addFieldListofInt(self,fieldList="FieldListofInt"):
        return
    @PYB11pycppname("addFieldList")
    def addFieldListofFloat(self,fieldList="FieldListofFloat"):
        return
    @PYB11pycppname("addFieldList")
    def addFieldListofString(self,fieldList="FieldListofString"):
        return
    
    numNodes = PYB11property("int", getter="getNumNodes", doc="The number of nodes in the nodeList.")
    count = PYB11property("int", getter="getFieldListCount", doc="The number of fieldLists in the nodeList.")
    fieldNames = PYB11property("std::vector<std::string>", getter="fieldNames", doc="The names of fieldLists in the nodeList.")