from PYB11Generator import *

@PYB11template("dim")
class Grid:
    def pyinit1(self,nx="int",sx="double"):
        return
    def pyinit2(self,nx="int",ny="int",sx="double",sy="double"):
        return
    def pyinit3(self,nx="int",ny="int",nz="int",sx="double",sy="double",sz="double"):
        return

    nx = PYB11property("double", getter="getnx", doc="The number of x coords.")
    ny = PYB11property("double", getter="getny", doc="The number of y coords.")
    nz = PYB11property("double", getter="getnz", doc="The number of z coords.")
    dx = PYB11property("double", getter="getdx", doc="The number of x coords.")
    dy = PYB11property("double", getter="getdy", doc="The number of y coords.")
    dz = PYB11property("double", getter="getdz", doc="The number of z coords.")