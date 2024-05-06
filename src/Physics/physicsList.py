from PYB11Generator import *

@PYB11template("dim")
class PhysicsList:
    def pyinit(self):
        return
    def addPackage(self,physics="Physics<%(dim)s>*"):
        return
    
    
PhysicsList1d = PYB11TemplateClass(PhysicsList,
                              template_parameters = ("1"),
                              cppname = "PhysicsList<1>",
                              pyname = "PhysicsList1d",
                              docext = " (1D).")
PhysicsList2d = PYB11TemplateClass(PhysicsList,
                              template_parameters = ("2"),
                              cppname = "PhysicsList<2>",
                              pyname = "PhysicsList2d",
                              docext = " (2D).")
PhysicsList3d = PYB11TemplateClass(PhysicsList,
                              template_parameters = ("3"),
                              cppname = "PhysicsList<3>",
                              pyname = "PhysicsList3d",
                              docext = " (3D).") 