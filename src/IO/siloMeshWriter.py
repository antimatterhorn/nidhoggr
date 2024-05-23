from PYB11Generator import *

@PYB11template("dim")
class SiloMeshWriter:
    def pyinit(self,
               baseName="std::string&",
               nodeList="NodeList&",
               fieldNames="std::vector<std::string>&"):
        return
    def write(self,fileName="std::string&"):
        return

SiloMeshWriter1d = PYB11TemplateClass(SiloMeshWriter,
                              template_parameters = ("1"),
                              cppname = "SiloMeshWriter<1>",
                              pyname = "SiloMeshWriter1d",
                              docext = " (1D).")
SiloMeshWriter2d = PYB11TemplateClass(SiloMeshWriter,
                              template_parameters = ("2"),
                              cppname = "SiloMeshWriter<2>",
                              pyname = "SiloMeshWriter2d",
                              docext = " (2D).")
SiloMeshWriter3d = PYB11TemplateClass(SiloMeshWriter,
                              template_parameters = ("3"),
                              cppname = "SiloMeshWriter<3>",
                              pyname = "SiloMeshWriter3d",
                              docext = " (3D).") 