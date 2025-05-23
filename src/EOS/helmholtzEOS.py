from PYB11Generator import *
from equationOfState import *
from EOSAbstractMethods import *

class HelmholtzEOS(EquationOfState):
    def pyinit(self,tableFile="std::string&",constants="PhysicalConstants&"):
        return

#-------------------------------------------------------------------------------
# Add the virtual interface
#-------------------------------------------------------------------------------
PYB11inject(EOSAbstractMethods, EquationOfState, pure_virtual=True)