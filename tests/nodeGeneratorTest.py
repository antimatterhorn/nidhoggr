import sys
import numpy as np
import matplotlib.pyplot as plt
import random
from nidhoggr import *

method = "PS"

n = 500

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

if method == "RPR":
    from RPRPSNodeGenerator import RecursivePrimitiveRefinementSurface3d
    points = RecursivePrimitiveRefinementSurface3d(n).positions
elif method == "PS":
    from RPRPSNodeGenerator import ParameterizedSpiralSurface3d
    points = ParameterizedSpiralSurface3d(n).positions
elif method == "AltAz":
    from AltAzNodeGenerator import AltAzSurface3d
    points = AltAzSurface3d(n).positions
elif method == "SEAGen":
    from SEANodeGenerator import SEAGenSurface3d
    points = SEAGenSurface3d(n).positions

from scipy.spatial import distance_matrix

points = np.array(points)

# Compute pairwise distances on the sphere
dists = distance_matrix(points, points)

# Define a Gaussian-like weight function
sigma = 0.1  # Tune this for best results
weights = np.exp(-dists**2 / (2 * sigma**2))

# Normalize so that the sum of weights approximates the total area (4π for a unit sphere)
local_density = weights.sum(axis=1)
As = (4 * np.pi) / local_density    # Approximate area per point
As = As/As.mean()                   # Normalize areas

print("Standard Deviation from Equal Area: %3.4f"%(np.std(As)))

# Extract coordinates
xs = points[:, 0]
ys = points[:, 1]
zs = points[:, 2]

# 3D Scatter plot
ax.scatter(xs, ys, zs, c=As, cmap="viridis")
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
ax.set_title(method)

# Create a separate 2D scatter plot of lattitude vs. area
# This is with knowledge that the worst portions of any tiling method are at the poles
fig2 = plt.figure()
ax2 = fig2.add_subplot()
ax2.scatter(zs, As, c=As, cmap="viridis")
ax2.set_xlabel("Z Coordinate")
ax2.set_ylabel("Area per Point (As)")
ax2.set_title("Z Coordinate vs Area")

plt.show()