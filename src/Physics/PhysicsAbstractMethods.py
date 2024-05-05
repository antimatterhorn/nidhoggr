#-------------------------------------------------------------------------------
# Helper to inject common virtual methods for equations of state
#-------------------------------------------------------------------------------
from PYB11Generator import *

@PYB11ignore
class PhysicsAbstractMethods:
    @PYB11const
    def UpdateDerivatives(self,
                    dt = "const double"):
        return "void"