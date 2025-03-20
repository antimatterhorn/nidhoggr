from PYB11Generator import *
from gridBoundaries import *

@PYB11holder("std::unique_ptr")
@PYB11template("dim")
class ReflectingGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return