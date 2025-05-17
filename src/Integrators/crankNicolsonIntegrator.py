from PYB11Generator import *
from integrator import *

@PYB11template("dim")
class CrankNicolsonIntegrator(Integrator):
    def pyinit(self,
               packages="std::vector<Physics<%(dim)s>*>",
               dtmin="double"):
        return
    def pyinit1(self,
               packages="std::vector<Physics<%(dim)s>*>",
               dtmin="double", verbose="bool"):
        return
    def Step(self):
        return
    
CrankNicolsonIntegrator1d = PYB11TemplateClass(CrankNicolsonIntegrator,
                              template_parameters = ("1"),
                              cppname = "CrankNicolsonIntegrator<1>",
                              pyname = "CrankNicolsonIntegrator1d",
                              docext = " (1D).")
CrankNicolsonIntegrator2d = PYB11TemplateClass(CrankNicolsonIntegrator,
                              template_parameters = ("2"),
                              cppname = "CrankNicolsonIntegrator<2>",
                              pyname = "CrankNicolsonIntegrator2d",
                              docext = " (2D).")
CrankNicolsonIntegrator3d = PYB11TemplateClass(CrankNicolsonIntegrator,
                              template_parameters = ("3"),
                              cppname = "CrankNicolsonIntegrator<3>",
                              pyname = "CrankNicolsonIntegrator3d",
                              docext = " (3D).") 