import sys
import numpy as np
import matplotlib.pyplot as plt
import random
from nidhoggr import *

n = 100

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

# from RPRPSNodeGenerator import ParameterizedSpiralSurface3d, RecursivePrimitiveRefinementSurface3d

# points = RecursivePrimitiveRefinementSurface3d(n).positions

# print("Generated %d points, when asked to generate %d."%(len(points),n))
# xs = []
# ys = []
# zs = []
# for p in points:
#     xs.append(p[0])
#     ys.append(p[1])
#     zs.append(p[2])

# ax.scatter(xs, ys, zs)

from SEANodeGenerator import SEANodeGeneratorSurface3d

gen = SEANodeGeneratorSurface3d(n)

plt.show()