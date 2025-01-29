from DataBase import *
from LinearAlgebra import *
import numpy as np
from numpy import pi

class SEANodeGeneratorSurface3d:
    # generates a single shell of particles on a unit sphere using the SEA generator from Kegerreis et al. 2019
    def __init__(self,nshell):
        self.positions = []
        self.Areg = 4.0*pi/nshell                # area of a region
        self.Hcap = 2*np.arcsin(np.sqrt(1.0/nshell))  # cap height
        self.Ncol = round((pi-2.0*self.Hcap)/self.Areg)    # number of collars
        self.Hcol = (pi-2.0*self.Hcap)/self.Ncol           # collar height

        self.collars = np.arange(self.Ncol,dtype='float')
        self.collars *= self.Hcol

        self.collars += self.Hcap

        self.collarAreas = np.empty(self.Ncol)
        self.collarAreas[0] = self.Areg
        self.collarAreas[1:] = self.getCollarArea(self.collars[1:],self.collars[:-1])

        self.reg = np.empty(self.Ncol, dtype=int)

        loop = enumerate(
            np.nditer(self.reg, op_flags=["readwrite"])
        )
        
        discrepancy = 0

        for i, Ni in loop:
            ideal_number = self.collarAreas[i]/self.Areg
            Ni[...] = int(round(ideal_number + discrepancy))
            discrepancy = Ni - ideal_number

        self.sum = np.cumsum(self.reg)

        #self.collars = 2.0*np.arcsin(np.sqrt(self.sum * self.Areg / (4*pi)))

        self.newN = self.reg.sum()
        self.theta = np.empty(self.newN)
        self.phi   = np.empty(self.newN)

        return

    def getCollarArea(self,ti,tj):
        sti = np.sin(ti/2.0)**2
        stj = np.sin(tj/2.0)**2
        return 4*np.pi*(sti+stj)
