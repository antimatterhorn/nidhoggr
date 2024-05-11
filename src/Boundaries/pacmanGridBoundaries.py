from PYB11Generator import *
from gridBoundaries import *

@PYB11template("dim")
class PacmanGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*",physics="Physics<%(dim)s>*"):
        return