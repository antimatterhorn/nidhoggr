from nidhoggr import *

nx = 10
ny = 10
dx = 0.1
dy = 0.1

nodeList = NodeList(nx*ny)
myGrid = Grid2d(nx,ny,dx,dy)

myGrid.assignPositions(nodeList)

positions = nodeList.getFieldVector2d("position")

tree = KDTree2d(positions)

point = positions[int(nx/2*ny/2)]
nbrs = tree.findNearestNeighbors(point,0.11)
print(point,"is point %d"%int(nx/2*ny/2),"has neighbors",nbrs)
for i in range(len(nbrs)):
    print(positions[nbrs[i]])

