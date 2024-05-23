from PYB11Generator import *

@PYB11template("dim")
class VTKMeshWriter:
    def pyinit(self,
               baseName="std::string&",
               nodeList="NodeList*",
               fieldNames="std::vector<std::string>&"):
        return
    def write(self,fileName="std::string&"):
        return

VTKMeshWriter1d = PYB11TemplateClass(VTKMeshWriter,
                              template_parameters = ("1"),
                              cppname = "VTKMeshWriter<1>",
                              pyname = "VTKMeshWriter1d",
                              docext = " (1D).")
VTKMeshWriter2d = PYB11TemplateClass(VTKMeshWriter,
                              template_parameters = ("2"),
                              cppname = "VTKMeshWriter<2>",
                              pyname = "VTKMeshWriter2d",
                              docext = " (2D).")
VTKMeshWriter3d = PYB11TemplateClass(VTKMeshWriter,
                              template_parameters = ("3"),
                              cppname = "VTKMeshWriter<3>",
                              pyname = "VTKMeshWriter3d",
                              docext = " (3D).") 