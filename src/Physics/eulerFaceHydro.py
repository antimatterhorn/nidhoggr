from PYB11Generator import *
from hydro import *

@PYB11template("dim")
class EulerFaceHydro(Hydro):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               grid="Mesh::Grid<%(dim)s>*"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int",fieldName="std::string"):
        return


EulerFaceHydro1d = PYB11TemplateClass(EulerFaceHydro,
                              template_parameters = ("1"),
                              cppname = "EulerFaceHydro<1>",
                              pyname = "EulerFaceHydro1d",
                              docext = " (1D).")
EulerFaceHydro2d = PYB11TemplateClass(EulerFaceHydro,
                              template_parameters = ("2"),
                              cppname = "EulerFaceHydro<2>",
                              pyname = "EulerFaceHydro2d",
                              docext = " (2D).")
EulerFaceHydro3d = PYB11TemplateClass(EulerFaceHydro,
                              template_parameters = ("3"),
                              cppname = "EulerFaceHydro<3>",
                              pyname = "EulerFaceHydro3d",
                              docext = " (3D).") 