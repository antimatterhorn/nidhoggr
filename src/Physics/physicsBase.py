from PYB11Generator import *

@PYB11template("dim")
class PhysicsBase:
    def pyinit(self,dataBase="DataBase*",constants="PhysicalConstants&"):
        return
    
    
PhysicsBase1d = PYB11TemplateClass(PhysicsBase,
                              template_parameters = ("1"),
                              cppname = "PhysicsBase<1>",
                              pyname = "PhysicsBase1d",
                              docext = " (1D).")
PhysicsBase2d = PYB11TemplateClass(PhysicsBase,
                              template_parameters = ("2"),
                              cppname = "PhysicsBase<2>",
                              pyname = "PhysicsBase2d",
                              docext = " (2D).")
PhysicsBase3d = PYB11TemplateClass(PhysicsBase,
                              template_parameters = ("3"),
                              cppname = "PhysicsBase<3>",
                              pyname = "PhysicsBase3d",
                              docext = " (3D).") 