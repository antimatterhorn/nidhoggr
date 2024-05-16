import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

def AnimateGrid2d(bounds, update_method, threeColors=False, frames=100, interval=50, scale=1, extremis=None, cmap='winter'):
    if (threeColors):
        fig, ax = plt.subplots()

        x = bounds[0]
        y = bounds[1]

        def update(frame):
            update_method.module_stepper()
            ax.clear()

            # Generate RGB values for each cell
            rgb_grid = np.zeros((x*scale,y*scale))
            if(threeColors):
                rgb_grid = np.zeros((x*scale, y*scale, 3))

            for j in range(y*scale):
                for i in range(x*scale):
                    rgb_grid[i, j] = update_method.module_call(i%x,j%y)
            # Plot the grid
            ax.imshow(rgb_grid, origin='lower', extent=[0, x*scale, 0, y*scale], interpolation='nearest')
            ax.set_title(update_method.module_title())
    else:
        fig, (ax_top, ax_bottom) = plt.subplots(2, 1, gridspec_kw={'height_ratios': [10, 1]})

        x = bounds[0]
        y = bounds[1]

        def update(frame):
            update_method.module_stepper()
            ax_top.clear()
            ax_bottom.clear()

            # Generate RGB values for each cell
            rgb_grid = np.zeros((x*scale,y*scale))
            if(threeColors):
                rgb_grid = np.zeros((x*scale, y*scale, 3))

            max_values = []
            for j in range(y*scale):
                maxi = 0
                for i in range(x*scale):
                    rgb_grid[i, j] = update_method.module_call(i%x,j%y)
                    if (i == x*scale/2):
                        maxi = rgb_grid[i,j]
                max_values.append(maxi)
            # Plot the grid
            if (extremis):
                ax_top.imshow(rgb_grid, origin='lower', extent=[0, x*scale, 0, y*scale], cmap=cmap, interpolation='nearest', 
                        vmin=extremis[0], vmax=extremis[1])
            else:
                ax_top.imshow(rgb_grid, origin='lower', extent=[0, x*scale, 0, y*scale], cmap=cmap, interpolation='nearest')
            ax_bottom.plot(range(x), max_values, color='blue')
            ax_bottom.set_xlim(0, x)
            ax_bottom.set_ylim(min(max_values)*1.1, max(max_values) * 1.1)
            ax_top.set_title(update_method.module_title())

    ani = FuncAnimation(fig, update, frames=frames, interval=interval)
    plt.show()

def AnimatePoints(points):
    fig, ax = plt.subplots()
    plt.show()

class AnimationUpdateMethod2d:
    def __init__(self, call, stepper, title=None):
        self.module_call = call
        self.module_stepper = stepper
        if (title == None):
            self.module_title = DummyTitle()
        else:
            self.module_title = title

    def __call__(self, i, j):
        return self.module_call(i, j)
    
class DummyTitle:
    def __init__(self):
        return
    def __call__(self):
        return "--"
    
class MakeTitle:
    def __init__(self, obj, var, name):
        self.obj = obj
        self.var = var
        self.name = name
    def __call__(self):
        member_value = getattr(self.obj, self.var)
        return f"%s: %03.3e" % (self.name,member_value)