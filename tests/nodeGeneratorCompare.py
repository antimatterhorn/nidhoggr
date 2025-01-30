import sys
import numpy as np
import matplotlib.pyplot as plt
import random
from nidhoggr import *
from scipy.spatial import distance_matrix


def stDev(method):
    ns = []
    sd = []
    for n in range(10,2000,10):
        p = method(n).positions
        p = np.array(p)

        # Compute pairwise distances on the sphere
        dists = distance_matrix(p, p)

        # Define a Gaussian-like weight function
        sigma = 0.1  # Tune this for best results
        weights = np.exp(-dists**2 / (2 * sigma**2))

        # Normalize so that the sum of weights approximates the total area (4π for a unit sphere)
        local_density = weights.sum(axis=1)
        As = (4 * np.pi) / local_density    # Approximate area per point
        As = As/As.mean()                   # Normalize areas

        ns.append(n)
        sd.append(np.std(As))
        print(n)
    return ns,sd

fig = plt.figure()
ax = fig.add_subplot()

from RPRPSNodeGenerator import RecursivePrimitiveRefinementSurface3d
method = RecursivePrimitiveRefinementSurface3d

xs,ys = stDev(method)
scatter1 = ax.scatter(xs, ys, c="red", label="RPR")

from RPRPSNodeGenerator import ParameterizedSpiralSurface3d
method = ParameterizedSpiralSurface3d

xs,ys = stDev(method)
scatter2 = ax.scatter(xs, ys, c="green", label="PS")

from AltAzNodeGenerator import AltAzNodeGeneratorSurface3d
method = AltAzNodeGeneratorSurface3d

xs,ys = stDev(method)
scatter3 = ax.scatter(xs, ys, c="blue", label="AltAz")

ax.set_xlabel("number of points")
ax.set_ylabel("standard deviation")

# Add legend
ax.legend()

plt.show()