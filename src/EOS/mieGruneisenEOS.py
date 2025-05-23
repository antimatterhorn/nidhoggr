from PYB11Generator import *
from equationOfState import *
from EOSAbstractMethods import *

class MieGruneisenEOS(EquationOfState):
    def pyinit(self,rho0="double",C0="double",Gamma0="double",S="double",constants="PhysicalConstants&"):
        return

#-------------------------------------------------------------------------------
# Add the virtual interface
#-------------------------------------------------------------------------------
PYB11inject(EOSAbstractMethods, EquationOfState, pure_virtual=True)
    
PYB11implementation("mieGruneisenEOS.cc")