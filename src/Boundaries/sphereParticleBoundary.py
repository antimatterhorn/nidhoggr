from PYB11Generator import *
from collider import *

@PYB11template("dim")
class SphereParticleBoundary(Collider):
    def pyinit(self,physics="Physics<%(dim)s>*",
                    position="Lin::Vector<%(dim)s>&",
                    radius="double"):
        return
    
SphereParticleBoundary1d = PYB11TemplateClass(SphereParticleBoundary,
                              template_parameters = ("1"),
                              cppname = "SphereParticleBoundary<1>",
                              pyname = "SphereParticleBoundary1d",
                              docext = " (1D).")
SphereParticleBounday2d = PYB11TemplateClass(SphereParticleBoundary,
                              template_parameters = ("2"),
                              cppname = "SphereParticleBoundary<2>",
                              pyname = "SphereParticleBoundary2d",
                              docext = " (2D).")
SphereParticleBoundary3d = PYB11TemplateClass(SphereParticleBoundary,
                              template_parameters = ("3"),
                              cppname = "SphereParticleBoundary<3>",
                              pyname = "SphereParticleBoundary3d",
                              docext = " (3D).") 