from PYB11Generator import *
from physics import *

@PYB11template("dim")
class ConstantGridAccel(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               gravityVector="Lin::Vector<%(dim)s>&"):
        return

ConstantGridAccel1d = PYB11TemplateClass(ConstantGridAccel,
                              template_parameters = ("1"),
                              cppname = "ConstantGridAccel<1>",
                              pyname = "ConstantGridAccel1d",
                              docext = " (1D).")
ConstantGridAccel2d = PYB11TemplateClass(ConstantGridAccel,
                              template_parameters = ("2"),
                              cppname = "ConstantGridAccel<2>",
                              pyname = "ConstantGridAccel2d",
                              docext = " (2D).")
ConstantGridAccel3d = PYB11TemplateClass(ConstantGridAccel,
                              template_parameters = ("3"),
                              cppname = "ConstantGridAccel<3>",
                              pyname = "ConstantGridAccel3d",
                              docext = " (3D).") 