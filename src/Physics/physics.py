from PYB11Generator import *

@PYB11template("dim")
class Physics:
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&"):
        return
    # @PYB11pure_virtual
    # def UpdateDerivatives(self,dt="const double"):
    #     return "void"
    
    
Physics1d = PYB11TemplateClass(Physics,
                              template_parameters = ("1"),
                              cppname = "Physics<1>",
                              pyname = "Physics1d",
                              docext = " (1D).")
Physics2d = PYB11TemplateClass(Physics,
                              template_parameters = ("2"),
                              cppname = "Physics<2>",
                              pyname = "Physics2d",
                              docext = " (2D).")
Physics3d = PYB11TemplateClass(Physics,
                              template_parameters = ("3"),
                              cppname = "Physics<3>",
                              pyname = "Physics3d",
                              docext = " (3D).") 