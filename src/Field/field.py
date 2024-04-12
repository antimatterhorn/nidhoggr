from PYB11Generator import *

@PYB11template("typename T")
class Field:
    def pyinit(self,name="std::string",value="%(typename T)s"):
        return

    name = PYB11property("std::string", getter="getName", doc="The name of the field.")

# VectorField = PYB11TemplateClass(Field,
#                               template_parameters = ("Vector<dim>"),
#                               cppname = "Field<Vector<dim>>",
#                               pyname = "VectorField",
#                               docext = " (Vector).")