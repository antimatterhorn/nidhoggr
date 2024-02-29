import importlib
import matplotlib.pyplot as plt

Grid = importlib.import_module('RDE')

print(dir(Grid))
grid = Grid.Grid()
grid.initialize()

# Run simulation for a certain number of steps
for step in range(100):
    grid.update()

# Visualize the concentrations using matplotlib
plt.imshow(grid.getX1())
plt.colorbar()
plt.show()
