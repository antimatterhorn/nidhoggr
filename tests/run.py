import importlib
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

Grid = importlib.import_module('RDE')

n = 3
x = 100
y = 100
grid = Grid.Grid(n, x, y)
grid.initialize()
grid.A = 0.8
grid.D = 0.4

fig, ax = plt.subplots()

def update(frame):
    grid.update()
    ax.clear()
    # Generate RGB values for each cell
    rgb_grid = np.zeros((x, y, 3))
    for i in range(x):
        for j in range(y):
            rgb_grid[i, j] = grid.getCell(i, j)
    # Plot the grid
    ax.imshow(rgb_grid, origin='lower', extent=[0, x, 0, y], interpolation='nearest')

ani = FuncAnimation(fig, update, frames=100, interval=50)
plt.show()
