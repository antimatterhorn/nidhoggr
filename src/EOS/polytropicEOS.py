from PYB11Generator import *
from equationOfState import *
from EOSAbstractMethods import *

class PolytropicEOS(EquationOfState):
    def pyinit(self,specificHeatRatio="double",constants="PhysicalConstants&"):
        return
    
    gamma = PYB11property("double", getter="getGamma", doc="Specific heat ratio.")
    kappa = PYB11property("double", getter="getKappa", doc="Polytropic constant.")
    index = PYB11property("double", getter="getIndex", doc="Polytropic index.")

#-------------------------------------------------------------------------------
# Add the virtual interface
#-------------------------------------------------------------------------------
PYB11inject(EOSAbstractMethods, EquationOfState, pure_virtual=True)
    
