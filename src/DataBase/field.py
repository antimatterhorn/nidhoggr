from PYB11Generator import *

# PYB11includes = ['"../Mesh/element.hh"']
# PYB11namespaces = ["Mesh"]  # So 'Mesh::Element<1>' is a valid type

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
        "Number of field elements"
        return "unsigned"
    

    @PYB11cppname("operator[]")
    @PYB11returnpolicy("reference_internal")
    @PYB11implementation('[](Field<FieldType>& self, int i) { const int n = self.size(); if (i >= n) throw py::index_error(); return &self[(i %% n + n) %% n]; }')
    def __getitem__(self):
        return

    # @PYB11cppname("setValue")
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

    def setValue(self,i="int",val="%(T)s"):
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

FieldofVector1d = PYB11TemplateClass(Field,
                              template_parameters = ("Lin::Vector<1>"),
                              cppname = "Field<Lin::Vector<1>>",
                              pyname = "FieldofVector1d",
                              docext = " (Lin::Vector<1>).")

FieldofVector2d = PYB11TemplateClass(Field,
                              template_parameters = ("Lin::Vector<2>"),
                              cppname = "Field<Lin::Vector<2>>",
                              pyname = "FieldofVector2d",
                              docext = " (Lin::Vector<2>).")

FieldofVector3d = PYB11TemplateClass(Field,
                              template_parameters = ("Lin::Vector<3>"),
                              cppname = "Field<Lin::Vector<3>>",
                              pyname = "FieldofVector3d",
                              docext = " (Lin::Vector<3>).")

FieldofTriangle = PYB11TemplateClass(Field,
                              template_parameters = ("Mesh::TriangleElement"),
                              cppname = "Field<Mesh::TriangleElement>",
                              pyname = "FieldofTriangleElement",
                              docext = " (Mesh::TriangleElement).")

FieldofQuad = PYB11TemplateClass(Field,
                              template_parameters = ("Mesh::QuadElement"),
                              cppname = "Field<Mesh::QuadElement>",
                              pyname = "FieldofQuadElement",
                              docext = " (Mesh::QuadElement).")