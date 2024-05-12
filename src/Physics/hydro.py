from PYB11Generator import *
from physics import *

@PYB11template("dim")
class Hydro(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&"):
        return
    # @PYB11virtual
    # def UpdateDerivatives(self,dt="const double"):
    #     return "void"

Hydro1d = PYB11TemplateClass(Hydro,
                              template_parameters = ("1"),
                              cppname = "Hydro<1>",
                              pyname = "Hydro1d",
                              docext = " (1D).")
Hydro2d = PYB11TemplateClass(Hydro,
                              template_parameters = ("2"),
                              cppname = "Hydro<2>",
                              pyname = "Hydro2d",
                              docext = " (2D).")
Hydro3d = PYB11TemplateClass(Hydro,
                              template_parameters = ("3"),
                              cppname = "Hydro<3>",
                              pyname = "Hydro3d",
                              docext = " (3D).") 