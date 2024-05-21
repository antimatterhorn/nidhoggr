from PYB11Generator import *
PYB11includes = ['"timeDilation.cc"','"cosmology.cc"']

class TimeDilation:
    def pyinit(self,
               travelTime="double",
               distance="double"):
        return
    def compute(self):
        return "void"

    velocity = PYB11property("double", getter="getVelocity", doc="The Earth-frame velocity in units of c.")
    earthTime = PYB11property("double", getter="getEarthTime", doc="The Earth-frame travel time in years.")

class Cosmology:
    def pyinit(self,
               z="double",
               omega_m="double",
               lam="double",
               h0="double"):
        return
    def compute(self):
        return "void"
    def output(self):
        return "void"