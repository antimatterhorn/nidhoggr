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
    @PYB11returnpolicy("reference")
    def getField(self,name="std::string"):
        return
    @PYB11returnpolicy("reference")
    def mass(self):
        return
    @PYB11template("dim")
    @PYB11returnpolicy("reference")
    def velocity(self):
        return 
    @PYB11template("dim")
    @PYB11returnpolicy("reference")
    def position(self):
        return
    numNodes = PYB11property("int", getter="getNumNodes", doc="The number of nodes in the nodeList.")
    count = PYB11property("int", getter="getFieldCount", doc="The number of fields in the nodeList.")
    fieldNames = PYB11property("std::vector<std::string>", getter="fieldNames", doc="The names of fields in the nodeList.")

    velocity1d = PYB11TemplateMethod(velocity,
                                template_parameters = ("1"),pyname="velocity")
    velocity2d = PYB11TemplateMethod(velocity,
                                template_parameters = ("2"),pyname="velocity")
    velocity3d = PYB11TemplateMethod(velocity,
                                template_parameters = ("3"),pyname="velocity")
    position1d = PYB11TemplateMethod(position,
                                template_parameters = ("1"),pyname="position")
    position2d = PYB11TemplateMethod(position,
                                template_parameters = ("2"),pyname="position")
    position3d = PYB11TemplateMethod(position,
                                template_parameters = ("3"),pyname="position")
    getFieldDouble = PYB11TemplateMethod(getField,
                                template_parameters = ("double"))
    getFieldVector1d = PYB11TemplateMethod(getField,
                                template_parameters = ("Lin::Vector<1>"))
    getFieldVector2d = PYB11TemplateMethod(getField,
                                template_parameters = ("Lin::Vector<2>"))
    getFieldVector3d = PYB11TemplateMethod(getField,
                                template_parameters = ("Lin::Vector<3>"))
    insertFieldDouble = PYB11TemplateMethod(insertField,
                                template_parameters = ("double"))