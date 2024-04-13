from PYB11Generator import *

@PYB11template("dim")
class HydroState:
    def pyinit(self):
        return
    def pyinit1(self,numNodes="int"):
        return

HydroState1d = PYB11TemplateClass(HydroState,
                              template_parameters = ("1"),
                              cppname = "HydroState<1>",
                              pyname = "HydroState1d",
                              docext = " (1D).")
HydroState2d = PYB11TemplateClass(HydroState,
                              template_parameters = ("2"),
                              cppname = "HydroState<2>",
                              pyname = "HydroState2d",
                              docext = " (2D).")
HydroState3d = PYB11TemplateClass(HydroState,
                              template_parameters = ("3"),
                              cppname = "HydroState<3>",
                              pyname = "HydroState3d",
                              docext = " (3D).") 