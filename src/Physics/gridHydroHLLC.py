from PYB11Generator import *
from hydro import *

@PYB11template("dim")
class GridHydroHLLC(Hydro):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<%(dim)s>*"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int",fieldName="std::string"):
        return


GridHydroHLLC1d = PYB11TemplateClass(GridHydroHLLC,
                              template_parameters = ("1"),
                              cppname = "GridHydroHLLC<1>",
                              pyname = "GridHydroHLLC1d",
                              docext = " (1D).")
GridHydroHLLC2d = PYB11TemplateClass(GridHydroHLLC,
                              template_parameters = ("2"),
                              cppname = "GridHydroHLLC<2>",
                              pyname = "GridHydroHLLC2d",
                              docext = " (2D).")
GridHydroHLLC3d = PYB11TemplateClass(GridHydroHLLC,
                              template_parameters = ("3"),
                              cppname = "GridHydroHLLC<3>",
                              pyname = "GridHydroHLLC3d",
                              docext = " (3D).") 