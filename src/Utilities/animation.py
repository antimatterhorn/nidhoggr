import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

def animate(bounds, update_method, frames=100, interval=50):
    fig, ax = plt.subplots()

    x = bounds[0]
    y = bounds[1]

    def update(frame):
        update_method.module_stepper()
        ax.clear()

        # Generate RGB values for each cell
        rgb_grid = np.zeros((x, y, 3))
        for i in range(x):
            for j in range(y):
                rgb_grid[i, j] = update_method.module_call(i,j)
        # Plot the grid
        ax.imshow(rgb_grid, origin='lower', extent=[0, x, 0, y], interpolation='nearest')

    ani = FuncAnimation(fig, update, frames=frames, interval=interval)
    plt.show()

class AnimationUpdateMethod:
    def __init__(self,module_call,module_stepper):
        self.module_call = module_call
        self.module_stepper = module_stepper
    def __call__(self,i,j):
        return self.module_call(i,j)