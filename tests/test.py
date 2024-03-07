import importlib
import matplotlib.pyplot as plt
import numpy as np

Grid = importlib.import_module('RDE')

n = 3
x = 100
y = 100
grid = Grid.Grid(n,x,y)
grid.initialize()
grid.A = 0.5
grid.D = 0.3

def plotGrid():
    # Generate RGB values for each cell
    rgb_grid = np.zeros((x, y, 3))
    for i in range(x):
        for j in range(y):
            rgb_grid[i, j] = grid.getCell(i, j)

    # Plot the grid
    plt.imshow(rgb_grid, origin='lower', extent=[0, x, 0, y], interpolation='nearest')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Grid with RGB colors')
    plt.colorbar(label='Color Intensity')
    plt.show()

#plotGrid()

grid.update()

#plotGrid()

# Run simulation for a certain number of steps
for step in range(100):
    grid.update()

plotGrid()