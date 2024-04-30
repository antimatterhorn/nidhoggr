from PYB11Generator import *

@PYB11template("dim")
class Grid:
    def pyinit1(self,nx="int",sx="double"):
        return
    def pyinit2(self,nx="int",ny="int",sx="double",sy="double"):
        return
    def pyinit3(self,nx="int",ny="int",nz="int",sx="double",sy="double",sz="double"):
        return
    # @PYB11pyname("state")
    # def getState(self,id="int"):
    #     return
    # @PYB11pyname("position")
    # def getPosition(self,id="int"):
    #     return
    # @PYB11pyname("positions")
    # def getPositions(self):
    #     return
    @PYB11pyname("position")
    def getPosition(self,id="int"):
        return
    def setOrigin(self,origin="Lin::Vector<%(dim)s>"):
        return

    nx = PYB11property("int", getter="getnx", doc="The number of x coords.")
    ny = PYB11property("int", getter="getny", doc="The number of y coords.")
    nz = PYB11property("int", getter="getnz", doc="The number of z coords.")
    dx = PYB11property("double", getter="getdx", doc="The number of x coords.")
    dy = PYB11property("double", getter="getdy", doc="The number of y coords.")
    dz = PYB11property("double", getter="getdz", doc="The number of z coords.")