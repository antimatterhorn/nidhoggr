from PYB11Generator import *
from gridBoundaries import *

@PYB11template("dim")
class ReflectingGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*"):
        return