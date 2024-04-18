from PYB11Generator import *

class PolytropicEOS:
    def pyinit(self,polytropicConstant="double",polytropicIndex="double",constants="PhysicalConstants&"):
        return
    def computePressure(self,density="double",internalEnergy="double"):
        return
    def computeInternalEnergy(self,density="double",pressure="double"):
        return
    def computeSoundSpeed(self,density="double",pressure="double"):
        return
    
    gamma = PYB11property("double", getter="getGamma", doc="Specific heat ratio.")
    kappa = PYB11property("double", getter="getKappa", doc="Polytropic constant.")
    index = PYB11property("double", getter="getIndex", doc="Polytropic index.")