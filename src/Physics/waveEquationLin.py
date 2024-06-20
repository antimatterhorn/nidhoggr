from PYB11Generator import *
from physics import *

@PYB11template("dim")
class WaveEquationLinear(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               grid="Mesh::Grid<%(dim)s>*",
               C="double"):
        return
    def pyinit1(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               grid="Mesh::Grid<2>*",
               depthMap="std::string&"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int",fieldName="std::string"):
        return

WaveEquationLinear1d = PYB11TemplateClass(WaveEquationLinear,
                              template_parameters = ("1"),
                              cppname = "WaveEquationLinear<1>",
                              pyname = "WaveEquationLinear1d",
                              docext = " (1D).")
WaveEquationLinear2d = PYB11TemplateClass(WaveEquationLinear,
                              template_parameters = ("2"),
                              cppname = "WaveEquationLinear<2>",
                              pyname = "WaveEquationLinear2d",
                              docext = " (2D).")
WaveEquationLinear3d = PYB11TemplateClass(WaveEquationLinear,
                              template_parameters = ("3"),
                              cppname = "WaveEquationLinear<3>",
                              pyname = "WaveEquationLinear3d",
                              docext = " (3D).") 