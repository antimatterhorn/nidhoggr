from PYB11Generator import *
from physics import *

@PYB11template()                                             # <--- force not to inherit template parameters from A
@PYB11template_dict({"dim" : "2"}) # <--- specify the template parameter substitutions
class WaveHeight(Physics):
    def pyinit(self,
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               grid="Mesh::Grid<2>*",
               depthMap="std::string&"):
        return
    @PYB11cppname("getCell")
    def getCell2d(self,i="int",j="int",fieldName="std::string"):
        return