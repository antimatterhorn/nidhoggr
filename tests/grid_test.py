from nidhoggr import *

myGrid = Grid3d(3,3,3,0.1,0.1,0.1)

print(myGrid.nx,myGrid.ny,myGrid.nz,myGrid.dx,myGrid.dy,myGrid.dz)
print(myGrid.positions()[0])