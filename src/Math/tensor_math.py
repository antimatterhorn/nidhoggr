from PYB11Generator import *

@PYB11template("dim")
class Tensor:
    "the tensor class for linear algebra"
    def pyinit(self):
        "default constructor"
        return
    def pyinit1(self,x="double"):
        return
    def pyinit2(self,xx="double",xy="double",yx="double",yy="double"):
        return
    def pyinit3(self,xx="double",xy="double",xz="double",yx="double",yy="double",yz="double",zx="double",zy="double",zz="double"):
        return 
    def __add__(self):
        return
    def __sub__(self):
        return
    def __neg__(self):
        return
    def __eq__(self):
        return
    def __ne__(self):
        return
    @PYB11pycppname("__mul__")
    def __mul__f(self,rhs="double()"):
        return 
    @PYB11pycppname("__rmul__")
    def __rmul__f(self,rhs="double()"):
        return 
    @PYB11cppname("toString")
    def __repr__(self):
        return

    xx = PYB11property("double", getter="xx", doc="The xx tensor component.")
    xy = PYB11property("double", getter="xy", doc="The xy tensor component.")
    xz = PYB11property("double", getter="xz", doc="The xz tensor component.")

    yx = PYB11property("double", getter="yx", doc="The yx tensor component.")
    yy = PYB11property("double", getter="yy", doc="The yy tensor component.")
    yz = PYB11property("double", getter="yz", doc="The yz tensor component.")

    zx = PYB11property("double", getter="zx", doc="The zx tensor component.")
    zy = PYB11property("double", getter="zy", doc="The zy tensor component.")
    zz = PYB11property("double", getter="zz", doc="The zz tensor component.")

    determinant = PYB11property("double", getter="determinant", doc="The determinant of the tensor.")