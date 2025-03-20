from PYB11Generator import *
from collider import *

@PYB11template("dim")
class BoxCollider(Collider):
    def pyinit(self,position1="Lin::Vector<%(dim)s>&",
                    position2="Lin::Vector<%(dim)s>&",
                    radius="double"):
        return
    def pyinit2(self,position1="Lin::Vector<%(dim)s>&",
                    position2="Lin::Vector<%(dim)s>&",
                    elasticity="double"):
        return
    
BoxCollider1d = PYB11TemplateClass(BoxCollider,
                              template_parameters = ("1"),
                              cppname = "BoxCollider<1>",
                              pyname = "BoxCollider1d",
                              docext = " (1D).")
BoxParticleBounday2d = PYB11TemplateClass(BoxCollider,
                              template_parameters = ("2"),
                              cppname = "BoxCollider<2>",
                              pyname = "BoxCollider2d",
                              docext = " (2D).")
BoxCollider3d = PYB11TemplateClass(BoxCollider,
                              template_parameters = ("3"),
                              cppname = "BoxCollider<3>",
                              pyname = "BoxCollider3d",
                              docext = " (3D).") 