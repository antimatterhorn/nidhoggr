from PYB11Generator import *
from physics import *

@PYB11template("dim")
class FEM(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               mesh="Mesh::FEMesh<%(dim)s>*"):
        return

FEM1d = PYB11TemplateClass(FEM,
                              template_parameters = ("1"),
                              cppname = "FEM<1>",
                              pyname = "FEM1d",
                              docext = " (1D).")
FEM2d = PYB11TemplateClass(FEM,
                              template_parameters = ("2"),
                              cppname = "FEM<2>",
                              pyname = "FEM2d",
                              docext = " (2D).")
FEM3d = PYB11TemplateClass(FEM,
                              template_parameters = ("3"),
                              cppname = "FEM<3>",
                              pyname = "FEM3d",
                              docext = " (3D).") 