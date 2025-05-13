from PYB11Generator import *
from hydro import *

@PYB11template("dim")
class GridHydroHLLE(Hydro):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<%(dim)s>*"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int",fieldName="std::string"):
        return


GridHydroHLLE1d = PYB11TemplateClass(GridHydroHLLE,
                              template_parameters = ("1"),
                              cppname = "GridHydroHLLE<1>",
                              pyname = "GridHydroHLLE1d",
                              docext = " (1D).")
GridHydroHLLE2d = PYB11TemplateClass(GridHydroHLLE,
                              template_parameters = ("2"),
                              cppname = "GridHydroHLLE<2>",
                              pyname = "GridHydroHLLE2d",
                              docext = " (2D).")
GridHydroHLLE3d = PYB11TemplateClass(GridHydroHLLE,
                              template_parameters = ("3"),
                              cppname = "GridHydroHLLE<3>",
                              pyname = "GridHydroHLLE3d",
                              docext = " (3D).") 