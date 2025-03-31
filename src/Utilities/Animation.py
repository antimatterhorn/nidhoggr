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

        nx = bounds[0]
        ny = bounds[1]

        def update(frame):
            update_method.module_stepper()
            ax.clear()

            # Generate RGB values for each cell
            rgb_grid = np.zeros((ny * scale, nx * scale))

            for j in range(ny * scale):
                for i in range(nx * scale):
                    rgb_grid[j,i] = update_method(i % nx, j % ny)
            # Plot the grid
            ax.imshow(rgb_grid, origin='lower', extent=[0, nx * scale, 0, ny * scale], interpolation='nearest')
            ax.set_title(update_method.module_title())
    else:
        fig, (ax_top, ax_bottom) = plt.subplots(2, 1, gridspec_kw={'height_ratios': [10, 1]})

        nx = bounds[0]
        ny = bounds[1]

        def update(frame):
            update_method.module_stepper()
            ax_top.clear()
            ax_bottom.clear()

            # Generate RGB values for each cell
            rgb_grid = np.zeros((ny * scale, nx * scale))  # Rows = y, Cols = x

            max_values = []
            for j in range(ny * scale):
                maxi = 0
                for i in range(nx * scale):
                    rgb_grid[j, i] = update_method(i % nx, j % ny)
                    if i == nx * scale // 2:
                        maxi = rgb_grid[j, i]

                max_values.append(maxi)
            # Plot the grid
            if extremis:
                ax_top.imshow(rgb_grid, origin='lower', extent=[0, nx * scale, 0, ny * scale], cmap=cmap, interpolation='nearest', 
                              vmin=extremis[0], vmax=extremis[1])
            else:
                ax_top.imshow(rgb_grid, origin='lower', extent=[0, nx * scale, 0, ny * scale], cmap=cmap, interpolation='nearest')
            ax_bottom.plot(range(ny * scale), max_values, color='blue')
            ax_bottom.set_xlim(0, ny * scale)
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

def AnimateScatter(bounds, stepper, positions, frames=100, interval=50, save_as=None):
    """
    Custom implementation of animate scatter just for this test.
    """
    fig, ax = plt.subplots()

    ax.set_xlim(bounds[0], bounds[1])
    ax.set_ylim(bounds[2], bounds[3])

    scat = ax.scatter([], [], label='Nodes')

    ax.legend()

    def init():
        scat.set_offsets(np.empty((0, 2)))  # Initialize with empty 2D array
        return scat,

    def update(frame):
        stepper.Step()  # Execute one step of the simulation

        points = [positions[i] for i in range(positions.size())]
        x = [p.x for p in points]
        y = [p.y for p in points]

        scat.set_offsets(np.c_[x, y])

        return scat,

    ani = FuncAnimation(fig, update, init_func=init, frames=frames, interval=interval, blit=True)

    if save_as:
        ani.save(save_as, writer='ffmpeg')
        print(f'Animation saved as {save_as}')
    else:
        plt.show()


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