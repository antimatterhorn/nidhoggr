from PYB11Generator import *

@PYB11template("dim")
class HDF5MeshWriter:
    def pyinit(self,
               baseName="std::string&",
               nodeList="NodeList*",
               fieldNames="std::vector<std::string>&"):
        return
    def write(self,fileName="std::string&"):
        return

HDF5MeshWriter1d = PYB11TemplateClass(HDF5MeshWriter,
                              template_parameters = ("1"),
                              cppname = "HDF5MeshWriter<1>",
                              pyname = "HDF5MeshWriter1d",
                              docext = " (1D).")
HDF5MeshWriter2d = PYB11TemplateClass(HDF5MeshWriter,
                              template_parameters = ("2"),
                              cppname = "HDF5MeshWriter<2>",
                              pyname = "HDF5MeshWriter2d",
                              docext = " (2D).")
HDF5MeshWriter3d = PYB11TemplateClass(HDF5MeshWriter,
                              template_parameters = ("3"),
                              cppname = "HDF5MeshWriter<3>",
                              pyname = "HDF5MeshWriter3d",
                              docext = " (3D).") 