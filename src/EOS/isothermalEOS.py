from PYB11Generator import *
from equationOfState import *
from EOSAbstractMethods import *

class IsothermalEOS(EquationOfState):
    def pyinit(self,soundSpeed="double",constants="PhysicalConstants&"):
        return

#-------------------------------------------------------------------------------
# Add the virtual interface
#-------------------------------------------------------------------------------
PYB11inject(EOSAbstractMethods, EquationOfState, pure_virtual=True)
    
