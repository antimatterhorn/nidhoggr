from PYB11Generator import *
from hydro import *

@PYB11template("dim")
class GridHydroHLL(Hydro):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<%(dim)s>*"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int",fieldName="std::string"):
        return


GridHydroHLL1d = PYB11TemplateClass(GridHydroHLL,
                              template_parameters = ("1"),
                              cppname = "GridHydroHLL<1>",
                              pyname = "GridHydroHLL1d",
                              docext = " (1D).")
GridHydroHLL2d = PYB11TemplateClass(GridHydroHLL,
                              template_parameters = ("2"),
                              cppname = "GridHydroHLL<2>",
                              pyname = "GridHydroHLL2d",
                              docext = " (2D).")
GridHydroHLL3d = PYB11TemplateClass(GridHydroHLL,
                              template_parameters = ("3"),
                              cppname = "GridHydroHLL<3>",
                              pyname = "GridHydroHLL3d",
                              docext = " (3D).") 