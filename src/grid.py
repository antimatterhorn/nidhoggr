from PYB11Generator import *

class Grid:
    def pyinit(self,n="int",size_x="int",size_y="int"):
        return
    def initialize(self):
        return
    def update(self):
        return
    def print(self):
        return
    def getCell(self,i="int",j="int"):
        return
    
    A = PYB11property("double", getter="getA", setter="setA", doc="reaction coefficient")
    D = PYB11property("double", getter="getD", setter="setD", doc="diffusion coefficient")
    #xx = PYB11property("copy", getter="getxx", doc="The data.")