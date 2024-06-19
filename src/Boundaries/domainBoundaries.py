from PYB11Generator import *
from boundaries import *

@PYB11template("dim")
class DomainBoundaries(Boundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return
    
DomainBoundaries1d = PYB11TemplateClass(DomainBoundaries,
                              template_parameters = ("1"),
                              cppname = "DomainBoundaries<1>",
                              pyname = "DomainBoundaries1d",
                              docext = " (1D).")
DomainBoundaries2d = PYB11TemplateClass(DomainBoundaries,
                              template_parameters = ("2"),
                              cppname = "DomainBoundaries<2>",
                              pyname = "DomainBoundaries2d",
                              docext = " (2D).")
DomainBoundaries3d = PYB11TemplateClass(DomainBoundaries,
                              template_parameters = ("3"),
                              cppname = "DomainBoundaries<3>",
                              pyname = "DomainBoundaries3d",
                              docext = " (3D).") 