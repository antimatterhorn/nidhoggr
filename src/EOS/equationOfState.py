from PYB11Generator import *
from EOSAbstractMethods import *

class EquationOfState:
    def pyinit(self,constants="PhysicalConstants&"):
        return
    
#-------------------------------------------------------------------------------
# Add the virtual interface
#-------------------------------------------------------------------------------
PYB11inject(EOSAbstractMethods, EquationOfState, pure_virtual=True)