from PYB11Generator import *
from gridBoundaries import *

@PYB11template("dim")
class DirichletGridBoundaries(GridBoundaries):
    def pyinit(self,grid="Mesh::Grid<%(dim)s>*"):
        return
    def addBox(self,p1="Lin::Vector<%(dim)s>",p2="Lin::Vector<%(dim)s>"):
        return
    def removeBox(self,p1="Lin::Vector<%(dim)s>",p2="Lin::Vector<%(dim)s>"):
        return
    def addSphere(self,p="Lin::Vector<%(dim)s>",radius="double"):
        return
    def removeSphere(self,p="Lin::Vector<%(dim)s>",radius="double"):
        return
    def addDomain(self):
        return
    def boundaryIds(self):
        return "std::vector<int>"
    
DirichletGridBoundaries1d = PYB11TemplateClass(DirichletGridBoundaries,
                              template_parameters = ("1"),
                              cppname = "DirichletGridBoundaries<1>",
                              pyname = "DirichletGridBoundaries1d",
                              docext = " (1D).")
DirichletGridBoundaries2d = PYB11TemplateClass(DirichletGridBoundaries,
                              template_parameters = ("2"),
                              cppname = "DirichletGridBoundaries<2>",
                              pyname = "DirichletGridBoundaries2d",
                              docext = " (2D).")
DirichletGridBoundaries3d = PYB11TemplateClass(DirichletGridBoundaries,
                              template_parameters = ("3"),
                              cppname = "DirichletGridBoundaries<3>",
                              pyname = "DirichletGridBoundaries3d",
                              docext = " (3D).") 