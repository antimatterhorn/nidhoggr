from PYB11Generator import *
from hydro import *

@PYB11template("dim")
class HydroHLL(Hydro):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<%(dim)s>*"):
        return


HydroHLL1d = PYB11TemplateClass(HydroHLL,
                              template_parameters = ("1"),
                              cppname = "HydroHLL<1>",
                              pyname = "HydroHLL1d",
                              docext = " (1D).")
HydroHLL2d = PYB11TemplateClass(HydroHLL,
                              template_parameters = ("2"),
                              cppname = "HydroHLL<2>",
                              pyname = "HydroHLL2d",
                              docext = " (2D).")
HydroHLL3d = PYB11TemplateClass(HydroHLL,
                              template_parameters = ("3"),
                              cppname = "HydroHLL<3>",
                              pyname = "HydroHLL3d",
                              docext = " (3D).") 