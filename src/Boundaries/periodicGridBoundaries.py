from PYB11Generator import *
from gridBoundaries import *

@PYB11holder("std::unique_ptr")
@PYB11template("dim")
class PeriodicGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return
    
PeriodicGridBoundaries1d = PYB11TemplateClass(PeriodicGridBoundaries,
                              template_parameters = ("1"),
                              cppname = "PeriodicGridBoundaries<1>",
                              pyname = "PeriodicGridBoundaries1d",
                              docext = " (1D).")
PeriodicGridBoundaries2d = PYB11TemplateClass(PeriodicGridBoundaries,
                              template_parameters = ("2"),
                              cppname = "PeriodicGridBoundaries<2>",
                              pyname = "PeriodicGridBoundaries2d",
                              docext = " (2D).")
PeriodicGridBoundaries3d = PYB11TemplateClass(PeriodicGridBoundaries,
                              template_parameters = ("3"),
                              cppname = "PeriodicGridBoundaries<3>",
                              pyname = "PeriodicGridBoundaries3d",
                              docext = " (3D).") 