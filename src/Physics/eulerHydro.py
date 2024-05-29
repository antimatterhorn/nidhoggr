from PYB11Generator import *
from hydro import *

@PYB11template("dim")
class EulerHydro(Hydro):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<%(dim)s>*"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int"):
        return


EulerHydro1d = PYB11TemplateClass(EulerHydro,
                              template_parameters = ("1"),
                              cppname = "EulerHydro<1>",
                              pyname = "EulerHydro1d",
                              docext = " (1D).")
EulerHydro2d = PYB11TemplateClass(EulerHydro,
                              template_parameters = ("2"),
                              cppname = "EulerHydro<2>",
                              pyname = "EulerHydro2d",
                              docext = " (2D).")
EulerHydro3d = PYB11TemplateClass(EulerHydro,
                              template_parameters = ("3"),
                              cppname = "EulerHydro<3>",
                              pyname = "EulerHydro3d",
                              docext = " (3D).") 