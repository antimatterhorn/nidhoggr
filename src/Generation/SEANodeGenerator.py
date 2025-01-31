from DataBase import *
from LinearAlgebra import *
import numpy as np
from numpy import pi

class SEAGenSurface3d:
    # generates a single shell of particles on a unit sphere using the SEA generator from Kegerreis et al. 2019
    def __init__(self,nshell):
        self.positions = []
        self.Areg = 4.0*pi/nshell                       # area of a region
        self.Hcap = 2*np.arcsin(np.sqrt(1.0/nshell))    # cap height
        self.Ncol = round((pi-2.0*self.Hcap)/np.sqrt(self.Areg)) # number of collars
        self.Hcol = (pi-2.0*self.Hcap)/self.Ncol        # collar height

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

        self.collars = 2.0 * np.arcsin(
            np.sqrt(np.clip(self.sum * self.Areg / (4*np.pi), 0, 1))
        )

        self.newN = self.reg.sum()
        self.theta = np.empty(self.newN)
        self.phi   = np.empty(self.newN)

        theta = np.zeros(self.Ncol)

        theta[0] = 0.0
        theta[1:] = 0.5 * (self.collars[:-1] + self.collars[1:])

        dphi = 2*np.pi/self.reg
        phi0 = np.empty(self.Ncol)

        loop = enumerate(
            np.nditer(phi0, op_flags=["writeonly"])
        )

        for i,phi0_i in loop:
            try:
                phi0_i[...] - self.choosephi_0(
                    self.reg[i],
                    self.reg[i-1],
                    dphi[i],
                    dphi[i-1]
                )

                phi0_i[...] += dphi[i-1]
            except IndexError:
                phi0_i[...] = 0

        tot_covered = 0
        loop = enumerate(
            np.nditer(self.reg,op_flags=["readonly"])
        )

        for region, nparts in loop:
            upper_bound = nparts + tot_covered

            self.theta[tot_covered:upper_bound] = theta[region]

            j = np.arange(nparts,dtype=float)
            these_phi = phi0[region] + j*dphi[region]
            self.phi[tot_covered:upper_bound] = these_phi

            tot_covered = upper_bound

        self.theta %= np.pi
        self.phi %= 2*np.pi

        x = np.sin(self.theta) * np.cos(self.phi)
        y = np.sin(self.theta) * np.sin(self.phi)
        z = np.cos(self.theta)

        self.positions = np.vstack((x, y, z)).T


        return

    def getCollarArea(self,ti,tj):
        sti = np.sin(ti/2.0)**2
        stj = np.sin(tj/2.0)**2
        return 4*np.pi*(sti-stj)
    
    def choosephi_0(self,
            Ni: float,
            Ni_plus_one: float,
            dphi_i: float,
            dphi_i_plus_one: float,
        ) -> float:
        """
        Choose the value of phi0. This comes from the discussion paragraph
        after Equation 12.
        """

        Ni_even = (Ni % 2) - 1
        Ni_plus_one_even = (Ni_plus_one % 2) - 1

        if Ni_even != Ni_plus_one_even:
            # Exclusive or
            return 0.5 * (Ni_even * dphi_i +
                          Ni_plus_one_even * dphi_i_plus_one)
        else:
            return max(dphi_i, dphi_i_plus_one)
