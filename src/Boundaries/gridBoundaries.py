from PYB11Generator import *
from boundaries import *

@PYB11template("dim")
class GridBoundaries(Boundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*"):
        return
    
GridBoundaries1d = PYB11TemplateClass(GridBoundaries,
                              template_parameters = ("1"),
                              cppname = "GridBoundaries<1>",
                              pyname = "GridBoundaries1d",
                              docext = " (1D).")
GridBoundaries2d = PYB11TemplateClass(GridBoundaries,
                              template_parameters = ("2"),
                              cppname = "GridBoundaries<2>",
                              pyname = "GridBoundaries2d",
                              docext = " (2D).")
GridBoundaries3d = PYB11TemplateClass(GridBoundaries,
                              template_parameters = ("3"),
                              cppname = "GridBoundaries<3>",
                              pyname = "GridBoundaries3d",
                              docext = " (3D).") 