from PYB11Generator import *

@PYB11template("typename T")
class FieldList:
    def pyinit(self,fieldName="std::string",numFields="int"):
        return
    def addField(self,field="%(typename T)s"):
        return
    
    size = PYB11property("int", getter="getSize", doc="The size of the FieldList.")
    name = PYB11property("std::string", getter="getName", doc="The name of the FieldList.")

FieldListofInt = PYB11TemplateClass(FieldList,
                              template_parameters = ("int"),
                              cppname = "FieldList<int>",
                              pyname = "FieldListofInt",
                              docext = " (int).")
FieldListofDouble = PYB11TemplateClass(FieldList,
                              template_parameters = ("double"),
                              cppname = "FieldList<double>",
                              pyname = "FieldListofDouble",
                              docext = " (double).")
FieldListofFloat = PYB11TemplateClass(FieldList,
                              template_parameters = ("float"),
                              cppname = "FieldList<float>",
                              pyname = "FieldListofFloat",
                              docext = " (float).")
FieldListofString = PYB11TemplateClass(FieldList,
                              template_parameters = ("std::string"),
                              cppname = "FieldList<std::string>",
                              pyname = "FieldListofString",
                              docext = " (std::string).")