from nidhoggr import *
from StringArtPy import *
import matplotlib.pyplot as plt

commandLine = CommandLineArguments(lineWidth = 0.2,
                                   filename = "pi.png",
                                   nailCount = 157,
                                   windings = 800)

lineWidth = 0.2

sa = StringArtPy(filename,nailCount=nailCount,windings=windings,width=lineWidth)

i = 0
# Initialize lists to store x and y coordinates for plotting
x = []
y = []
while i < len(sa.pins)-1:
    pin0 = sa.pins[i]
    pin1 = sa.pins[i+1]
    point0 = sa.nails[pin0]
    point1 = sa.nails[pin1]

    x.extend([point0.x, point1.x])  # Add x coordinates of the line
    y.extend([point0.y, point1.y])  # Add y coordinates of the line

    i+=1

plt.figure()
plt.plot(x, y, marker='o', color='black',linewidth=lineWidth)  # Use marker='o' to plot points at each vertex of the line
plt.title('Lines Between Points')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
plt.show()

print(sa.length/sa.radius)