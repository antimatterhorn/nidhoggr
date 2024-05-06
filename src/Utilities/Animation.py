import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

def AnimateGrid2d(bounds, update_method, frames=100, interval=50, scale=1):
    fig, ax = plt.subplots()

    x = bounds[0]
    y = bounds[1]

    def update(frame):
        update_method.module_stepper()
        ax.clear()

        # Generate RGB values for each cell
        rgb_grid = np.zeros((x*scale, y*scale, 3))
        for i in range(x*scale):
            for j in range(y*scale):
                rgb_grid[i, j] = update_method.module_call(i%x,j%y)
        # Plot the grid
        ax.imshow(rgb_grid, origin='lower', extent=[0, x*scale, 0, y*scale], interpolation='nearest')

    ani = FuncAnimation(fig, update, frames=frames, interval=interval)
    plt.show()

def AnimatePoints(points):
    fig, ax = plt.subplots()
    plt.show()

class AnimationUpdateMethod2d:
    def __init__(self,call,stepper):
        self.module_call = call
        self.module_stepper = stepper
    def __call__(self,i,j):
        return self.module_call(i,j)