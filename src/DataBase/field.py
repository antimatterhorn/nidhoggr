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
    
    @PYB11virtual
    @PYB11const
    def size(self):
        "Number of elements"
        return "unsigned"

    @PYB11cppname("operator[]")
    @PYB11returnpolicy("reference_internal")
    @PYB11implementation('[](Field<FieldType>& self, int i) { const int n = self.size(); if (i >= n) throw py::index_error(); return &self[(i %% n + n) %% n]; }')
    def __getitem__(self):
        return

    # @PYB11implementation("[](Field<FieldType>& self, int i, const %(Value)s v) { const int n = self.size(); if (i >= n) throw py::index_error(); self[(i %% n + n) %% n] = v; }")
    # def __setitem__(self):
    #     "Set a value"

    # @PYB11implementation("[](const FieldType& self) { return py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0,1>()")
    # def __iter__(self):
    #     "Python iteration through a Field."

    @PYB11returnpolicy("reference_internal")
    @PYB11implementation("[](Field<FieldType>& self, int i) { const int n = self.size(); if (i >= n) throw py::index_error(); return &self[(i %% n + n) %% n]; }")
    def __call__(self):
        "Index into a Field"
        return


        
    name = PYB11property("std::string", getter="getNameString", doc="The name of the Field.")
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