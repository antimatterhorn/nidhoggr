from PYB11Generator import *
from physics import *

@PYB11template("dim")
class Kinetics(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&"):
        return

Kinetics1d = PYB11TemplateClass(Kinetics,
                              template_parameters = ("1"),
                              cppname = "Kinetics<1>",
                              pyname = "Kinetics1d",
                              docext = " (1D).")
Kinetics2d = PYB11TemplateClass(Kinetics,
                              template_parameters = ("2"),
                              cppname = "Kinetics<2>",
                              pyname = "Kinetics2d",
                              docext = " (2D).")
Kinetics3d = PYB11TemplateClass(Kinetics,
                              template_parameters = ("3"),
                              cppname = "Kinetics<3>",
                              pyname = "Kinetics3d",
                              docext = " (3D).") 