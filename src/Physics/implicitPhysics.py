from PYB11Generator import *
from physics import *

@PYB11template("dim")
class ImplicitPhysics(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&"):
        return

ImplicitPhysics1d = PYB11TemplateClass(ImplicitPhysics,
                              template_parameters = ("1"),
                              cppname = "ImplicitPhysics<1>",
                              pyname = "ImplicitPhysics1d",
                              docext = " (1D).")
ImplicitPhysics2d = PYB11TemplateClass(ImplicitPhysics,
                              template_parameters = ("2"),
                              cppname = "ImplicitPhysics<2>",
                              pyname = "ImplicitPhysics2d",
                              docext = " (2D).")
ImplicitPhysics3d = PYB11TemplateClass(ImplicitPhysics,
                              template_parameters = ("3"),
                              cppname = "ImplicitPhysics<3>",
                              pyname = "ImplicitPhysics3d",
                              docext = " (3D).") 