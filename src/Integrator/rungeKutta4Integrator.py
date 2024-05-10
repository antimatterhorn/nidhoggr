from PYB11Generator import *
from integrator import *

@PYB11template("dim")
class RungeKutta4Integrator(Integrator):
    def pyinit(self,
               physics="Physics<%(dim)s>*",
               dtmin="double",
               boundaries=("std::vector<Boundaries<%(dim)s>*>","std::vector<Boundaries<%(dim)s>*>()")):
        return
    def Step(self):
        return
    
RungeKutta4Integrator1d = PYB11TemplateClass(RungeKutta4Integrator,
                              template_parameters = ("1"),
                              cppname = "RungeKutta4Integrator<1>",
                              pyname = "RungeKutta4Integrator1d",
                              docext = " (1D).")
RungeKutta4Integrator2d = PYB11TemplateClass(RungeKutta4Integrator,
                              template_parameters = ("2"),
                              cppname = "RungeKutta4Integrator<2>",
                              pyname = "RungeKutta4Integrator2d",
                              docext = " (2D).")
RungeKutta4Integrator3d = PYB11TemplateClass(RungeKutta4Integrator,
                              template_parameters = ("3"),
                              cppname = "RungeKutta4Integrator<3>",
                              pyname = "RungeKutta4Integrator3d",
                              docext = " (3D).") 