from DataBase import *
from LinearAlgebra import *

class SEANodeGeneratorSurface3d:
    # generates a single shell of particles on a unit sphere using the SEA generator from Kegerreis et al. 2019
    def __init__(self,numNodes):
        self.positions = []
        Areg = 4.0*np.pi/numNodes
        tcap = 2*np.asin(sqrt(numNodes))
        Ncol = round((np.pi-2.0*tcap)/Areg)
        tcol = (np.pi-2.0*tcap)/Ncol

        return
