from PYB11Generator import *

class NodeList:
    def pyinit(self):
        return
    def pyinit1(self,numNodes= ("unsigned", "0")):
        return
    def addField(self,field="FieldBase*"):
        return
    def getField(self,name="std::string"):
        return
    
    numNodes = PYB11property("int", getter="getNumNodes", doc="The number of nodes in the nodeList.")
    count = PYB11property("int", getter="getFieldCount", doc="The number of fields in the nodeList.")
    fieldNames = PYB11property("std::vector<std::string>", getter="fieldNames", doc="The names of fields in the nodeList.")