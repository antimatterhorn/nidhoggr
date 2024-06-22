from PYB11Generator import *
PYB11includes = ['"timeDilation.cc"','"cosmology.cc"','"stringArt.cc"']

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

class StringArt:
    def pyinit(self,
               image="std::vector<std::vector<int>>",
               nails="std::vector<Lin::Vector<2>>",
               windings=("int",1000),
               mode=("int",0),
               width=("double",0.2),
               radius=("double",1.0)):
        return
    def PinList(self):
        return