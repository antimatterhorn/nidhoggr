from PYB11Generator import *
from boundaries import *

@PYB11template("dim")
class Collider(Boundaries):
    def pyinit(self):
        return
    
Collider1d = PYB11TemplateClass(Collider,
                              template_parameters = ("1"),
                              cppname = "Collider<1>",
                              pyname = "Collider1d",
                              docext = " (1D).")
Collider2d = PYB11TemplateClass(Collider,
                              template_parameters = ("2"),
                              cppname = "Collider<2>",
                              pyname = "Collider2d",
                              docext = " (2D).")
Collider3d = PYB11TemplateClass(Collider,
                              template_parameters = ("3"),
                              cppname = "Collider<3>",
                              pyname = "Collider3d",
                              docext = " (3D).") 