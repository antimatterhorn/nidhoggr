from PYB11Generator import *
from physics import *

class StellarEvolution(Physics):
    def pyinit(self,
               grid="Mesh::StellarGrid1d*",
               nodeList="NodeList*",
               constants="PhysicalConstants&",
               eos="EquationOfState*"):
        return