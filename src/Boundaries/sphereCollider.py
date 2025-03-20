from PYB11Generator import *
from collider import *

@PYB11template("dim")
class SphereCollider(Collider):
    def pyinit(self,position="Lin::Vector<%(dim)s>&",
                    radius="double"):
        return
    def pyinit2(self,position="Lin::Vector<%(dim)s>&",
                    radius="double",
                    elasticity="double"):
        return
    
SphereCollider1d = PYB11TemplateClass(SphereCollider,
                              template_parameters = ("1"),
                              cppname = "SphereCollider<1>",
                              pyname = "SphereCollider1d",
                              docext = " (1D).")
SphereParticleBounday2d = PYB11TemplateClass(SphereCollider,
                              template_parameters = ("2"),
                              cppname = "SphereCollider<2>",
                              pyname = "SphereCollider2d",
                              docext = " (2D).")
SphereCollider3d = PYB11TemplateClass(SphereCollider,
                              template_parameters = ("3"),
                              cppname = "SphereCollider<3>",
                              pyname = "SphereCollider3d",
                              docext = " (3D).") 