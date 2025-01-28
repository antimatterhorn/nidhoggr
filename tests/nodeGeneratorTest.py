import sys
import numpy as np
import matplotlib.pyplot as plt
import random
from nidhoggr import *

n = 80

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

from RPRPSNodeGenerator import ParameterizedSpiralSurface3d, RecursivePrimitiveRefinementSurface3d

points = RecursivePrimitiveRefinementSurface3d(n).positions
xs = []
ys = []
zs = []
for p in points:
    xs.append(p[0])
    ys.append(p[1])
    zs.append(p[2])

ax.scatter(xs, ys, zs)

plt.show()