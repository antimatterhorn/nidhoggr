from PYB11Generator import *

class NodeList:
    def pyinit(self):
        return
    def pyinit1(self,numNodes= ("unsigned", "0")):
        return
    def addField(self,field="FieldBase*"):
        return
    @PYB11template("typename T")
    def insertField(self,name="std::string"):
        return
    @PYB11template("typename T")
    def getField(self,name="std::string"):
        return "<%(T)s>"
    def mass(self):
        return
    @PYB11template("int dim")
    def velocity(self):
        return "Vector<%(dim)s>"
    @PYB11template("int dim")
    def position(self):
        return "Vector<%(dim)s>"
    numNodes = PYB11property("int", getter="getNumNodes", doc="The number of nodes in the nodeList.")
    count = PYB11property("int", getter="getFieldCount", doc="The number of fields in the nodeList.")
    fieldNames = PYB11property("std::vector<std::string>", getter="fieldNames", doc="The names of fields in the nodeList.")