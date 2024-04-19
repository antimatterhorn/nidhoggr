from PYB11Generator import *


#-------------------------------------------------------------------------------
# FieldBase
#-------------------------------------------------------------------------------
class FieldListBase:
    "Base class for FieldLists -- not much to implement in Python."

@PYB11template("typename T")
class FieldList(FieldListBase):
    
    PYB11typedefs = """
    typedef FieldList<%(T)s> FieldListType;
    typedef %(T)s FieldType;
"""
       
    def pyinit(self,fieldName="std::string"):
        return
    def addField(self,field="FieldType"):
        return
    
    @PYB11cppname("operator[]")
    @PYB11keepalive(0,1)
    def __getitem__(self, index="const unsigned"):
        return "FieldType"

    size = PYB11property("int", getter="getSize", doc="The size of the FieldList.")
    name = PYB11property("std::string", getter="getName", doc="The name of the FieldList.")
    fields = PYB11property("std::vector<FieldType>", getter="getFields", doc="The fields.")

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