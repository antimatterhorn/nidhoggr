#-------------------------------------------------------------------------------
# Helper to inject common virtual methods for equations of state
#-------------------------------------------------------------------------------
from PYB11Generator import *

@PYB11ignore
class EOSAbstractMethods:
    # Field methods
    # --------------
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
    
    # Scalar methods
    # --------------
    @PYB11const
    @PYB11pycppname("setPressure")
    def setPressure1(self,
                    pressure = "double*",
                    density = "double*",
                    internalEnergy = "double*"):
        return "void"
    
    @PYB11const
    @PYB11pycppname("setInternalEnergy")
    def setInternalEnergy1(self,
                    internalEnergy = "double*",
                    density = "double*",
                    pressure = "double*"):
        return "void"
    
    @PYB11const
    @PYB11pycppname("setSoundSpeed")
    def setSoundSpeed1(self,
                    soundSpeed = "double*",
                    density = "double*",
                    internalEnergy = "double*"):
        return "void"
    
    @PYB11const
    @PYB11pycppname("setTemperature")
    def setTemperature1(self,
                    temperature  = "double*",
                    density = "double*",
                    internalEnergy = "double*"):
        return "void"
    
    @PYB11const
    @PYB11pycppname("setInternalEnergyFromTemperature")
    def setInternalEnergyFromTemperature1(self,
                    internalEnergy = "double*",
                    density = "double*",
                    temperature = "double*"):
        return "void"
    
    @PYB11const
    @PYB11pycppname("getGamma")
    def getGamma(self):
        return "double"