from PYB11Generator import *

class TimeDilation:
    def pyinit(self,
               travelTime="double",
               distance="double"):
        return
    def compute(self):
        return "void"

    velocity = PYB11property("double", getter="getVelocity", doc="The Earth-frame velocity in units of c.")
    earthTime = PYB11property("double", getter="getEarthTime", doc="The Earth-frame travel time in years.")