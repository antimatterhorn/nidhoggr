#-------------------------------------------------------------------------------
# Helper to inject common virtual methods for equations of state
#-------------------------------------------------------------------------------
from PYB11Generator import *

@PYB11ignore
class OpacAbstractMethods:
    # Field methods
    # --------------
    @PYB11const
    def setOpacity(self,
                    opacity = "Field<double>*",
                    density = "Field<double>*",
                    temperature = "Field<double>*"):
        return "void"
    
    @PYB11const
    def setConductivity(self,
                    conductivity = "Field<double>*",
                    density = "Field<double>*",
                    temperature  = "Field<double>*"):
        return "void"
    
    # Scalar methods
    # --------------
    @PYB11const
    @PYB11pycppname("setOpacity")
    def setOpacity1(self,
                    opacity = "double*",
                    density = "double*",
                    temperature = "double*"):
        return "void"
    
    @PYB11const
    @PYB11pycppname("setConductivity")
    def setConductivity1(self,
                    conductivity = "double*",
                    density = "double*",
                    pressure = "double*"):
        return "void"
    
    @PYB11const
    @PYB11pycppname("name")
    def name(self):
        return "std::string"