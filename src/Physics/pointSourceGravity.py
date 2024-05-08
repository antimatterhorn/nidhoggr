from PYB11Generator import *
from physics import *

@PYB11template("dim")
class PointSourceGravity(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               pointSourceLocation="Lin::Vector<%(dim)s>&",
               pointSourceMass="double"):
        return
    # @PYB11virtual
    # def UpdateDerivatives(self,dt="const double"):
    #     return "void"

PointSourceGravity1d = PYB11TemplateClass(PointSourceGravity,
                              template_parameters = ("1"),
                              cppname = "PointSourceGravity<1>",
                              pyname = "PointSourceGravity1d",
                              docext = " (1D).")
PointSourceGravity2d = PYB11TemplateClass(PointSourceGravity,
                              template_parameters = ("2"),
                              cppname = "PointSourceGravity<2>",
                              pyname = "PointSourceGravity2d",
                              docext = " (2D).")
PointSourceGravity3d = PYB11TemplateClass(PointSourceGravity,
                              template_parameters = ("3"),
                              cppname = "PointSourceGravity<3>",
                              pyname = "PointSourceGravity3d",
                              docext = " (3D).") 