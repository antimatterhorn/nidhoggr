from PYB11Generator import *

@PYB11template("dim")
class KDTree:
    def pyinit(self,
               points="Field<Lin::Vector<%(dim)s>>*"):
        return
    def findNearestNeighbors(self,
                             points="const Lin::Vector<%(dim)s>&",
                             radius="double"):
        return "std::vector<int>"

KDTree1d = PYB11TemplateClass(KDTree,
                              template_parameters = ("1"),
                              cppname = "KDTree<1>",
                              pyname = "KDTree1d",
                              docext = " (1D).")
KDTree2d = PYB11TemplateClass(KDTree,
                              template_parameters = ("2"),
                              cppname = "KDTree<2>",
                              pyname = "KDTree2d",
                              docext = " (2D).")
KDTree3d = PYB11TemplateClass(KDTree,
                              template_parameters = ("3"),
                              cppname = "KDTree<3>",
                              pyname = "KDTree3d",
                              docext = " (3D).") 