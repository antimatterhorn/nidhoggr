from PYB11Generator import *
from physics import *

@PYB11template("dim")
class ConstantGravity(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               gravityVector="Lin::Vector<%(dim)s>&"):
        return
    # @PYB11virtual
    # def UpdateDerivatives(self,dt="const double"):
    #     return "void"

ConstantGravity1d = PYB11TemplateClass(ConstantGravity,
                              template_parameters = ("1"),
                              cppname = "ConstantGravity<1>",
                              pyname = "ConstantGravity1d",
                              docext = " (1D).")
ConstantGravity2d = PYB11TemplateClass(ConstantGravity,
                              template_parameters = ("2"),
                              cppname = "ConstantGravity<2>",
                              pyname = "ConstantGravity2d",
                              docext = " (2D).")
ConstantGravity3d = PYB11TemplateClass(ConstantGravity,
                              template_parameters = ("3"),
                              cppname = "ConstantGravity<3>",
                              pyname = "ConstantGravity3d",
                              docext = " (3D).") 