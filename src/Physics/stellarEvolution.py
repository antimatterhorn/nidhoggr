from PYB11Generator import *
from physics import *

@PYB11template()
@PYB11template_dict({"dim" : "1"})
class StellarEvolution(Physics):
    def pyinit(self,
               grid="Mesh::StellarGrid1d*",
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*",
               totalMass="double",
               centralTemperature="double"):
        return