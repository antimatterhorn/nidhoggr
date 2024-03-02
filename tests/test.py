import importlib
import matplotlib.pyplot as plt

Grid = importlib.import_module('RDE')

n = 3
x = 10
y = 10
grid = Grid.Grid(n,x,y)
grid.initialize()
grid.A = 0.2
grid.D = 0.7

grid.print()
print("")
grid.update()
grid.print()
print("")
# Run simulation for a certain number of steps
for step in range(100):
    grid.update()

grid.print()
# Visualize the concentrations using matplotlib
#plt.imshow(grid.getX1())
#plt.colorbar()
#plt.show()
