from PYB11Generator import *
from gridBoundaries import *

@PYB11template("dim")
class PacmanGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return
    
PacmanGridBoundaries1d = PYB11TemplateClass(PacmanGridBoundaries,
                              template_parameters = ("1"),
                              cppname = "PacmanGridBoundaries<1>",
                              pyname = "PacmanGridBoundaries1d",
                              docext = " (1D).")
PacmanGridBoundaries2d = PYB11TemplateClass(PacmanGridBoundaries,
                              template_parameters = ("2"),
                              cppname = "PacmanGridBoundaries<2>",
                              pyname = "PacmanGridBoundaries2d",
                              docext = " (2D).")
PacmanGridBoundaries3d = PYB11TemplateClass(PacmanGridBoundaries,
                              template_parameters = ("3"),
                              cppname = "PacmanGridBoundaries<3>",
                              pyname = "PacmanGridBoundaries3d",
                              docext = " (3D).") 