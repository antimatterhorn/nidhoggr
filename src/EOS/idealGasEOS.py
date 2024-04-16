from PYB11Generator import *

class IdealGasEOS:
    def pyinit(self,specificHeatRatio="double",constants="PhysicalConstants&"):
        return
    def computePressure(self,density="double",internalEnergy="double"):
        return
    def computeInternalEnergy(self,density="double",pressure="double"):
        return
    def computeSoundSpeed(self,density="double",pressure="double"):
        return
    
    gamma = PYB11property("double", getter="getGamma", doc="Specific heat ratio.")