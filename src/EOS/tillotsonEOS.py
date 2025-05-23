from PYB11Generator import *
from equationOfState import *
from EOSAbstractMethods import *

class TillotsonEOS(EquationOfState):
    def pyinit(self,
               rho0="double",
               A="double",
               B="double",
               alpha="double",
               beta="double",
               a="double",
               b="double",
               e0="double",
               eiv="double",
               ecv="double",constants="PhysicalConstants&"):
        return
    
#-------------------------------------------------------------------------------
# Add the virtual interface
#-------------------------------------------------------------------------------
PYB11inject(EOSAbstractMethods, EquationOfState, pure_virtual=True)

    
