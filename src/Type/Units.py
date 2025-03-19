from PhysicalConstants import *

#-------------------------------------------------------------------------------
# MKS units.
#-------------------------------------------------------------------------------
class MKS(PhysicalConstants):
    def __init__(self):
        PhysicalConstants.__init__(self, 
                                   1.0,   # Unit length (m)
                                   1.0,   # Unit mass (kg)
                                   1.0,   # Unit time (sec)
                                   1.0,   # Unit temp (kelvin)
                                   1.0)   # Unit charge (coulomb)
        return

#-------------------------------------------------------------------------------
# CGS units.
#-------------------------------------------------------------------------------
class CGS(PhysicalConstants):
    def __init__(self):
        PhysicalConstants.__init__(self, 
                                   0.01,  # Unit length (m)
                                   0.001, # Unit mass (kg)
                                   1.0,   # Unit time (sec)
                                   1.0,   # Unit temp (kelvin)
                                   1.0)   # Unit charge (coulomb)
        return
    
#-------------------------------------------------------------------------------
# CGS units.
#-------------------------------------------------------------------------------
class SOL(PhysicalConstants):
    def __init__(self):
        PhysicalConstants.__init__(self, 
                                   1.496e11,     # au m
                                   1.99e30,     # solar mass in  kg
                                   3.156e7          # year in s
                                   )   
        return