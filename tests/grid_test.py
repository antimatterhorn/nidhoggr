import sys
sys.path.append("Math")
sys.path.append("Field")
sys.path.append("Mesh")

from Mesh import *
from Field import *
from LinearAlgebra import *

myGrid = Grid3d(3,3,3,0.1,0.1,0.1)

print(myGrid.nx,myGrid.ny,myGrid.nz,myGrid.dx,myGrid.dy,myGrid.dz)
print(myGrid.positions()[0])