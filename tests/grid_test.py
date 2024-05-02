from nidhoggr import *

nx = 10
ny = 10
dx = 0.1
dy = 0.1

origin = Vector2d(0,0)

myGrid = Grid2d(nx,ny,dx,dy)
myGrid.setOrigin(origin)

print(myGrid.nx,myGrid.ny,myGrid.nz,myGrid.dx,myGrid.dy,myGrid.dz)
print(myGrid.position(0))