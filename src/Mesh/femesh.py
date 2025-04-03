from PYB11Generator import *

@PYB11template("dim")
class FEMesh:
    def pyinit(self):
        return
    def addNode(self,position="Lin::Vector<%(dim)s>"):
        return
    def getNodes(self):
        return "std::vector<Lin::Vector<dim>>"