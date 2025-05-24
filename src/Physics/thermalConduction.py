from PYB11Generator import *
from physics import *

@PYB11template("dim")
class ThermalConduction(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<dim>*"):
        return

ThermalConduction1d = PYB11TemplateClass(ThermalConduction,
                              template_parameters = ("1"),
                              cppname = "ThermalConduction<1>",
                              pyname = "ThermalConduction1d",
                              docext = " (1D).")
ThermalConduction2d = PYB11TemplateClass(ThermalConduction,
                              template_parameters = ("2"),
                              cppname = "ThermalConduction<2>",
                              pyname = "ThermalConduction2d",
                              docext = " (2D).")
ThermalConduction3d = PYB11TemplateClass(ThermalConduction,
                              template_parameters = ("3"),
                              cppname = "ThermalConduction<3>",
                              pyname = "ThermalConduction3d",
                              docext = " (3D).") 