from PYB11Generator import *

@PYB11template("dim")
class FEMesh:
    def pyinit(self):
        return
    def addNode(self,position="Lin::Vector<%(dim)s>"):
        return
    def getNodes(self):
        return "std::vector<Lin::Vector<%(dim)s>>"
    
FEMesh1d = PYB11TemplateClass(FEMesh,
                              template_parameters = ("1"),
                              cppname = "Mesh::FEMesh<1>",
                              pyname = "FEMesh1d",
                              docext = " (1D).")
FEMesh2d = PYB11TemplateClass(FEMesh,
                              template_parameters = ("2"),
                              cppname = "Mesh::FEMesh<2>",
                              pyname = "FEMesh2d",
                              docext = " (2D).")
FEMesh3d = PYB11TemplateClass(FEMesh,
                              template_parameters = ("3"),
                              cppname = "Mesh::FEMesh<3>",
                              pyname = "FEMesh3d",
                              docext = " (3D).")
