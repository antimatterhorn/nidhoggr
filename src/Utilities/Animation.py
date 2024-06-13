import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

def AnimateGrid2d(bounds, update_method, threeColors=False, frames=100, interval=50, scale=1, extremis=None, cmap='winter', save_as=None):
    """
    Creates an animation of a 2D grid.

    Parameters:
    - bounds: tuple of (x, y) dimensions for the grid
    - update_method: object with methods `module_stepper`, `module_call`, and `module_title`
    - threeColors: bool, whether to use three colors (RGB) for the grid
    - frames: int, number of frames in the animation
    - interval: int, time interval between frames in milliseconds
    - scale: int, scaling factor for the grid
    - extremis: tuple of (vmin, vmax) for color scale limits
    - cmap: str, colormap for the grid
    - save_as: str, file name to save the animation (e.g., 'animation.mp4')
    """
    if threeColors:
        fig, ax = plt.subplots()

        x = bounds[0]
        y = bounds[1]

        def update(frame):
            update_method.module_stepper()
            ax.clear()

            # Generate RGB values for each cell
            rgb_grid = np.zeros((x * scale, y * scale, 3))

            for j in range(y * scale):
                for i in range(x * scale):
                    rgb_grid[i, j] = update_method(i % x, j % y)
            # Plot the grid
            ax.imshow(rgb_grid, origin='lower', extent=[0, x * scale, 0, y * scale], interpolation='nearest')
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
            rgb_grid = np.zeros((x * scale, y * scale))

            max_values = []
            for j in range(y * scale):
                maxi = 0
                for i in range(x * scale):
                    rgb_grid[i, j] = update_method(i % x, j % y)
                    if i == x * scale / 2:
                        maxi = rgb_grid[i, j]
                max_values.append(maxi)
            # Plot the grid
            if extremis:
                ax_top.imshow(rgb_grid, origin='lower', extent=[0, x * scale, 0, y * scale], cmap=cmap, interpolation='nearest', 
                              vmin=extremis[0], vmax=extremis[1])
            else:
                ax_top.imshow(rgb_grid, origin='lower', extent=[0, x * scale, 0, y * scale], cmap=cmap, interpolation='nearest')
            ax_bottom.plot(range(x), max_values, color='blue')
            ax_bottom.set_xlim(0, x)
            ax_bottom.set_ylim(min(max_values) * 1.1, max(max_values) * 1.1)
            ax_top.set_title(update_method.module_title())

    ani = FuncAnimation(fig, update, frames=frames, interval=interval)

    if save_as:
        ani.save(save_as, writer='ffmpeg')
        print(f'Animation saved as {save_as}')
    else:
        plt.show()

# Example usage:
# AnimateGrid2d((10, 10), update_method, save_as='animation.mp4')

def AnimateScatter(bounds, update_method, frames=100, interval=50, save_as=None):
    """
    Creates an animation of a scatter plot of moving points.

    Parameters:
    - bounds: tuple of (x_min, x_max, y_min, y_max) dimensions for the plot
    - update_method: object with methods `module_stepper`, `module_call`, and `module_title`
    - frames: int, number of frames in the animation
    - interval: int, time interval between frames in milliseconds
    - save_as: str, file name to save the animation (e.g., 'animation.mp4')
    """
    fig, ax = plt.subplots()

    ax.set_xlim(bounds[0], bounds[1])
    ax.set_ylim(bounds[2], bounds[3])

    scat = ax.scatter([], [])

    def init():
        scat.set_offsets([])
        return scat,

    def update(frame):
        update_method.module_stepper()

        points = update_method.positions
        x = [p.x for p in points]
        y = [p.y for p in points]

        scat.set_offsets(np.c_[x, y])
        ax.set_title(update_method.module_title())

        return scat,

    ani = FuncAnimation(fig, update, init_func=init, frames=frames, interval=interval, blit=True)

    if save_as:
        ani.save(save_as, writer='ffmpeg')
        print(f'Animation saved as {save_as}')
    else:
        plt.show()

class AnimationUpdateScatterMethod2d:
    def __init__(self, positions, stepper, title=None):
        self.positions = positions
        self.module_stepper = stepper
        if (title == None):
            self.module_title = DummyTitle()
        else:
            self.module_title = title


class AnimationUpdateMethod2d:
    def __init__(self, call, stepper, title=None, fieldName="pressure"):
        self.module_call = call
        self.module_stepper = stepper
        self.fieldName = fieldName
        if (title == None):
            self.module_title = DummyTitle()
        else:
            self.module_title = title

    def __call__(self, i, j):
        return self.module_call(i, j,self.fieldName)
    
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