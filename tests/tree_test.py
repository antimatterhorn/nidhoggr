from nidhoggr import *

nx = 10
ny = 10
nz = 10
dx = 0.1
dy = 0.1
dz = 0.1

nodeList = NodeList(nx*ny*nz)
myGrid = Grid3d(nx,ny,nz,dx,dy,dz)

myGrid.assignPositions(nodeList)

positions = nodeList.getFieldVector3d("position")

tree = KDTree3d(positions)

point = positions[int(nx/2*ny/2*nz/2)]
nbrs = tree.findNearestNeighbors(point,1.1*dx)
print(point,"is point %d"%int(nx/2*ny/2*nz/2),"has neighbors",nbrs)
for i in range(len(nbrs)):
    print(positions[nbrs[i]])

