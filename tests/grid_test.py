from nidhoggr import *

commandLine = CommandLineArguments(nx = 5,
                                   ny = 8,
                                   dx = 0.1,
                                   dy = 0.1,
                                   ox = 0,
                                   oy = 0,
                                   ni = 8)


origin = Vector2d(ox,oy)

myGrid = Grid2d(nx,ny,dx,dy)
myGrid.setOrigin(origin)

dgb = DirichletGridBoundaries2d(grid=myGrid)
dgb.addDomain()

print("indices:")
for j in range(ny):
    row = []
    for i in range(nx):
        idx = myGrid.index(i,j,0)
        row.append(idx)
    print(", ".join(f"{num:02d}" for num in row))

print("bounds:")
for j in range(ny):
    row = []
    for i in range(nx):
        idx = myGrid.index(i,j,0)
        row.append(myGrid.onBoundary(idx))
    print(", ".join(f"{num:b}" for num in row))

print("\ndirichlet ids:")
print(dgb.boundaryIds())

print("\nleft:\t",", ".join(f"{num:02d}" for num in myGrid.leftMost()))
print("right:\t",", ".join(f"{num:02d}" for num in myGrid.rightMost()))
print("top:\t",", ".join(f"{num:02d}" for num in myGrid.topMost()))
print("bottom:\t",", ".join(f"{num:02d}" for num in myGrid.bottomMost()))

print("\npositions:")
for j in range(ny):
    row = []
    for i in range(nx):
        idx = myGrid.index(i,j,0)
        row.append(myGrid.position(idx))
    print(row)

print("\nneighboring cells:")
print(myGrid.getNeighboringCells(ni))