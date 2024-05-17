from PYB11Generator import *
from physics import *

@PYB11template("dim")
class SimplePhysics(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&"):
        return
    # @PYB11virtual
    # def UpdateDerivatives(self,dt="const double"):
    #     return "void"

SimplePhysics1d = PYB11TemplateClass(SimplePhysics,
                              template_parameters = ("1"),
                              cppname = "SimplePhysics<1>",
                              pyname = "SimplePhysics1d",
                              docext = " (1D).")
SimplePhysics2d = PYB11TemplateClass(SimplePhysics,
                              template_parameters = ("2"),
                              cppname = "SimplePhysics<2>",
                              pyname = "SimplePhysics2d",
                              docext = " (2D).")
SimplePhysics3d = PYB11TemplateClass(SimplePhysics,
                              template_parameters = ("3"),
                              cppname = "SimplePhysics<3>",
                              pyname = "SimplePhysics3d",
                              docext = " (3D).") 