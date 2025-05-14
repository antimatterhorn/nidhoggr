from PYB11Generator import *
from integrator import *

@PYB11template("dim")
class RungeKutta2Integrator(Integrator):
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
    
RungeKutta2Integrator1d = PYB11TemplateClass(RungeKutta2Integrator,
                              template_parameters = ("1"),
                              cppname = "RungeKutta2Integrator<1>",
                              pyname = "RungeKutta2Integrator1d",
                              docext = " (1D).")
RungeKutta2Integrator2d = PYB11TemplateClass(RungeKutta2Integrator,
                              template_parameters = ("2"),
                              cppname = "RungeKutta2Integrator<2>",
                              pyname = "RungeKutta2Integrator2d",
                              docext = " (2D).")
RungeKutta2Integrator3d = PYB11TemplateClass(RungeKutta2Integrator,
                              template_parameters = ("3"),
                              cppname = "RungeKutta2Integrator<3>",
                              pyname = "RungeKutta2Integrator3d",
                              docext = " (3D).") 