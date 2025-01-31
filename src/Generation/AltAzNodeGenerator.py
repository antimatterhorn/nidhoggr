from LinearAlgebra import *
from math import cos,sin,acos,asin,sqrt
from numpy import pi

class AltAzSurface3d:
    # generates a surface of particles on a unit sphere using PS
    def __init__(self,nshell):
        self.positions = []
        fib = 1.6180397959
        ai  = 2.0*pi*fib
        for i in range(nshell):
            t = i/(float)(nshell)
            inc = acos(-1+2*t)
            az = ai * i
            x = sin(inc)*cos(az)
            y = sin(inc)*sin(az)
            z = cos(inc)
            

            self.positions.append([x,y,z])
        
        return