from PYB11Generator import *
from gridBoundaries import *

@PYB11template("dim")
class PacmanDomainBoundaries(DomainBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return
    
PacmanDomainBoundaries1d = PYB11TemplateClass(PacmanDomainBoundaries,
                              template_parameters = ("1"),
                              cppname = "PacmanDomainBoundaries<1>",
                              pyname = "PacmanDomainBoundaries1d",
                              docext = " (1D).")
PacmanDomainBoundaries2d = PYB11TemplateClass(PacmanDomainBoundaries,
                              template_parameters = ("2"),
                              cppname = "PacmanDomainBoundaries<2>",
                              pyname = "PacmanDomainBoundaries2d",
                              docext = " (2D).")
PacmanDomainBoundaries3d = PYB11TemplateClass(PacmanDomainBoundaries,
                              template_parameters = ("3"),
                              cppname = "PacmanDomainBoundaries<3>",
                              pyname = "PacmanDomainBoundaries3d",
                              docext = " (3D).") 