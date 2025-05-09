from PYB11Generator import *
from gridBoundaries import *

@PYB11template("dim")
class ReflectingGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*"):
        return
    
ReflectingGridBoundaries1d = PYB11TemplateClass(ReflectingGridBoundaries,
                              template_parameters = ("1"),
                              cppname = "ReflectingGridBoundaries<1>",
                              pyname = "ReflectingGridBoundaries1d",
                              docext = " (1D).")
ReflectingGridBoundaries2d = PYB11TemplateClass(ReflectingGridBoundaries,
                              template_parameters = ("2"),
                              cppname = "ReflectingGridBoundaries<2>",
                              pyname = "ReflectingGridBoundaries2d",
                              docext = " (2D).")
ReflectingGridBoundaries3d = PYB11TemplateClass(ReflectingGridBoundaries,
                              template_parameters = ("3"),
                              cppname = "ReflectingGridBoundaries<3>",
                              pyname = "ReflectingGridBoundaries3d",
                              docext = " (3D).") 