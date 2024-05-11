from PYB11Generator import *

@PYB11template("dim")
class Boundaries:
    def pyinit(self,physics="Physics<%(dim)s>*"):
        return
    
Boundaries1d = PYB11TemplateClass(Boundaries,
                              template_parameters = ("1"),
                              cppname = "Boundaries<1>",
                              pyname = "Boundaries1d",
                              docext = " (1D).")
Boundaries2d = PYB11TemplateClass(Boundaries,
                              template_parameters = ("2"),
                              cppname = "Boundaries<2>",
                              pyname = "Boundaries2d",
                              docext = " (2D).")
Boundaries3d = PYB11TemplateClass(Boundaries,
                              template_parameters = ("3"),
                              cppname = "Boundaries<3>",
                              pyname = "Boundaries3d",
                              docext = " (3D).") 