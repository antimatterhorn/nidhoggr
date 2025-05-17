#-------------------------------------------------------------------------------
# Helper to inject common virtual methods for equations of state
#-------------------------------------------------------------------------------
from PYB11Generator import *

@PYB11ignore
class EOSAbstractMethods:
    @PYB11const
    def setPressure(self,
                    pressure = "Field<double>*",
                    density = "Field<double>*",
                    internalEnergy = "Field<double>*"):
        return "void"
    
    @PYB11const
    def setInternalEnergy(self,
                    internalEnergy = "Field<double>*",
                    density = "Field<double>*",
                    pressure = "Field<double>*"):
        return "void"
    
    @PYB11const
    def setSoundSpeed(self,
                    soundSpeed = "Field<double>*",
                    density = "Field<double>*",
                    internalEnergy = "Field<double>*"):
        return "void"
    
    @PYB11const
    def setTemperature(self,
                    temperature  = "Field<double>*",
                    density = "Field<double>*",
                    internalEnergy = "Field<double>*"):
        return "void"
    
    @PYB11const
    def setInternalEnergyFromTemperature(self,
                    internalEnergy = "Field<double>*",
                    density = "Field<double>*",
                    temperature = "Field<double>*"):
        return "void"