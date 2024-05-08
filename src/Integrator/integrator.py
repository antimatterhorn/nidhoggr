from PYB11Generator import *

@PYB11template("dim")
class Integrator:
    def pyinit(self,
               physics="Physics<%(dim)s>*"):
        return
    def Step(self,dt="double"):
        return
    def Time(self):
        return
    
Integrator1d = PYB11TemplateClass(Integrator,
                              template_parameters = ("1"),
                              cppname = "Integrator<1>",
                              pyname = "Integrator1d",
                              docext = " (1D).")
Integrator2d = PYB11TemplateClass(Integrator,
                              template_parameters = ("2"),
                              cppname = "Integrator<2>",
                              pyname = "Integrator2d",
                              docext = " (2D).")
Integrator3d = PYB11TemplateClass(Integrator,
                              template_parameters = ("3"),
                              cppname = "Integrator<3>",
                              pyname = "Integrator3d",
                              docext = " (3D).") 