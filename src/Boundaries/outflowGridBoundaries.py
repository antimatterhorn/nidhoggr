from PYB11Generator import *
from gridBoundaries import *

@PYB11template("dim")
class OutflowGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return
    
OutflowGridBoundaries1d = PYB11TemplateClass(OutflowGridBoundaries,
                              template_parameters = ("1"),
                              cppname = "OutflowGridBoundaries<1>",
                              pyname = "OutflowGridBoundaries1d",
                              docext = " (1D).")
OutflowGridBoundaries2d = PYB11TemplateClass(OutflowGridBoundaries,
                              template_parameters = ("2"),
                              cppname = "OutflowGridBoundaries<2>",
                              pyname = "OutflowGridBoundaries2d",
                              docext = " (2D).")
OutflowGridBoundaries3d = PYB11TemplateClass(OutflowGridBoundaries,
                              template_parameters = ("3"),
                              cppname = "OutflowGridBoundaries<3>",
                              pyname = "OutflowGridBoundaries3d",
                              docext = " (3D).") 