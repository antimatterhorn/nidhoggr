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
    def size(self):
        return

    nx = PYB11property("int", getter="getnx", doc="The number of x coords.")
    ny = PYB11property("int", getter="getny", doc="The number of y coords.")
    nz = PYB11property("int", getter="getnz", doc="The number of z coords.")
    dx = PYB11property("double", getter="getdx", doc="The number of x coords.")
    dy = PYB11property("double", getter="getdy", doc="The number of y coords.")
    dz = PYB11property("double", getter="getdz", doc="The number of z coords.")

Grid1d = PYB11TemplateClass(Grid,
                              template_parameters = ("1"),
                              cppname = "Mesh::Grid<1>",
                              pyname = "Grid1d",
                              docext = " (1D).")
Grid2d = PYB11TemplateClass(Grid,
                              template_parameters = ("2"),
                              cppname = "Mesh::Grid<2>",
                              pyname = "Grid2d",
                              docext = " (2D).")
Grid3d = PYB11TemplateClass(Grid,
                              template_parameters = ("3"),
                              cppname = "Mesh::Grid<3>",
                              pyname = "Grid3d",
                              docext = " (3D).") 