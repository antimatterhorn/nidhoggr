from PYB11Generator import *


#-------------------------------------------------------------------------------
# FieldBase
#-------------------------------------------------------------------------------
class FieldBase:
    "Base class for Fields -- not much to implement in Python."

@PYB11template("typename T")
class Field(FieldBase):
    
    PYB11typedefs = """
    typedef %(T)s FieldType;
"""
       
    def pyinit(self,fieldName="std::string"):
        return
    def addValue(self,field="FieldType"):
        return
    
    @PYB11cppname("operator[]")
    @PYB11keepalive(0,1)
    def __getitem__(self, index="const unsigned"):
        return "FieldType"

    size = PYB11property("int", getter="getSize", doc="The size of the Field.")
    name = PYB11property("std::string", getter="getName", doc="The name of the Field.")
    values = PYB11property("std::vector<FieldType>", getter="getValues", doc="The values.")

FieldofInt = PYB11TemplateClass(Field,
                              template_parameters = ("int"),
                              cppname = "Field<int>",
                              pyname = "FieldofInt",
                              docext = " (int).")
FieldofDouble = PYB11TemplateClass(Field,
                              template_parameters = ("double"),
                              cppname = "Field<double>",
                              pyname = "FieldofDouble",
                              docext = " (double).")
FieldofFloat = PYB11TemplateClass(Field,
                              template_parameters = ("float"),
                              cppname = "Field<float>",
                              pyname = "FieldofFloat",
                              docext = " (float).")
FieldofString = PYB11TemplateClass(Field,
                              template_parameters = ("std::string"),
                              cppname = "Field<std::string>",
                              pyname = "FieldofString",
                              docext = " (std::string).")