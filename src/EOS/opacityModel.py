from PYB11Generator import *
from OpacAbstractMethods import *

class OpacityModel:
    def pyinit(self,constants="PhysicalConstants&"):
        return
    
#-------------------------------------------------------------------------------
# Add the virtual interface
#-------------------------------------------------------------------------------
PYB11inject(OpacAbstractMethods, OpacityModel, pure_virtual=True)