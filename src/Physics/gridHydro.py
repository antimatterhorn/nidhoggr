from PYB11Generator import *
from hydro import *

@PYB11template("dim")
class GridHydro(Hydro):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<%(dim)s>*"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int"):
        return


GridHydro1d = PYB11TemplateClass(GridHydro,
                              template_parameters = ("1"),
                              cppname = "GridHydro<1>",
                              pyname = "GridHydro1d",
                              docext = " (1D).")
GridHydro2d = PYB11TemplateClass(GridHydro,
                              template_parameters = ("2"),
                              cppname = "GridHydro<2>",
                              pyname = "GridHydro2d",
                              docext = " (2D).")
GridHydro3d = PYB11TemplateClass(GridHydro,
                              template_parameters = ("3"),
                              cppname = "GridHydro<3>",
                              pyname = "GridHydro3d",
                              docext = " (3D).") 