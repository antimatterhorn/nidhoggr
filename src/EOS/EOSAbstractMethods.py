#-------------------------------------------------------------------------------
# Helper to inject common virtual methods for equations of state
#-------------------------------------------------------------------------------
from PYB11Generator import *

@PYB11ignore
class EOSAbstractMethods:
    @PYB11const
    def setPressure(self,
                    pressure = "Field<double>&",
                    density = "const Field<double>&",
                    internalEnergy = "const Field<double>&"):
        return "void"
    
    @PYB11const
    def setInternalEnergy(self,
                    internalEnergy = "Field<double>&",
                    density = "const Field<double>&",
                    pressure = "const Field<double>&"):
        return "void"
    
    @PYB11const
    def setSoundSpeed(self,
                    soundSpeed = "Field<double>&",
                    density = "const Field<double>&",
                    internalEnergy = "const Field<double>&"):
        return "void"