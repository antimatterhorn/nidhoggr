from nidhoggr import *
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import random

def AnimateScatter(bounds, stepper, positions, colliders, frames=100, interval=50):
    """
    Custom implementation of animate scatter just for this test.
    """
    fig, ax = plt.subplots()

    ax.set_xlim(bounds[0], bounds[1])
    ax.set_ylim(bounds[2], bounds[3])

    scat = ax.scatter([], [], label='Nodes')
    col_scat = ax.scatter([], [], color='red', s=100, label='Colliders')  # Set size of colliders

    ax.legend()

    def init():
        scat.set_offsets(np.empty((0, 2)))  # Initialize with empty 2D array
        col_scat.set_offsets(np.array(colliders))  # Initialize colliders
        return scat, col_scat

    def update(frame):
        stepper.Step()  # Execute one step of the simulation

        points = [positions[i] for i in range(positions.size())]
        x = [p.x for p in points]
        y = [p.y for p in points]

        scat.set_offsets(np.c_[x, y])

        col_x = [c[0] for c in colliders]
        col_y = [c[1] for c in colliders]
        col_scat.set_offsets(np.c_[col_x, col_y])

        return scat, col_scat

    ani = FuncAnimation(fig, update, init_func=init, frames=frames, interval=interval, blit=True)

    plt.show()

if __name__ == "__main__":
    animate = True
    
    numNodes = 100

    constants = MKS()
    myNodeList = NodeList(numNodes)
    gravVec = Vector2d(0, -9.8)

    constantGravity = ConstantGravity2d(myNodeList, constants, gravVec)
    packages = [constantGravity]

    colliders = []
    cbounds = []
    spacing_x = 2  # Horizontal spacing between colliders
    spacing_y = (6 - 2) / 5  # Vertical spacing between colliders to span from 2 to 8
    rows = 8
    collider_radius = 0.3

    for i in range(rows):
        num_colliders = 10 if i % 2 == 0 else 9
        offset = 0 if i % 2 == 0 else spacing_x / 2  # Stagger every other row
        for j in range(num_colliders):
            x = (j - num_colliders // 2) * spacing_x + offset
            y = 2 + i * spacing_y  # Start from y=2 and span to y=8
            cbounds.append(SphereCollider2d(physics=constantGravity, position=Vector2d(x, y), radius=collider_radius, elasticity=0.8))
            colliders.append((x, y))

    integrator = RungeKutta2Integrator2d(packages=packages, dtmin=0.01, boundaries=cbounds)
    print(integrator)

    myNodeList.insertFieldDouble("radius")
    rad = myNodeList.getFieldDouble("radius")
    for i in range(numNodes):
        rad.setValue(i, 0.2)

    print("numNodes =", myNodeList.numNodes)
    print("field names =", myNodeList.fieldNames)

    pos = myNodeList.getFieldVector2d("position")

    for i in range(numNodes):
        pos.setValue(i, Vector2d(random.uniform(-10, 10), 10))

    controller = Controller(integrator=integrator, periodicWork=[], statStep=100)

    bounds = (-10, 10, 0, 10)
    AnimateScatter(bounds, stepper=controller, positions=pos, colliders=colliders, frames=100, interval=50)
