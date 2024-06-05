from PYB11Generator import *
from physics import *

@PYB11template("dim")
class NBodyGravity(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               plummerLength="double"):
        return

NBodyGravity1d = PYB11TemplateClass(NBodyGravity,
                              template_parameters = ("1"),
                              cppname = "NBodyGravity<1>",
                              pyname = "NBodyGravity1d",
                              docext = " (1D).")
NBodyGravity2d = PYB11TemplateClass(NBodyGravity,
                              template_parameters = ("2"),
                              cppname = "NBodyGravity<2>",
                              pyname = "NBodyGravity2d",
                              docext = " (2D).")
NBodyGravity3d = PYB11TemplateClass(NBodyGravity,
                              template_parameters = ("3"),
                              cppname = "NBodyGravity<3>",
                              pyname = "NBodyGravity3d",
                              docext = " (3D).") 