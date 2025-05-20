from PYB11Generator import *

class StellarGrid1d:
    def pyinit(self,numZones="int"):
        return
    def size(self):
        return
    def dm(self,i="int") -> "double":
        return
    
    m = PYB11property("std::vector<double>&", getter="getMasses")
    r = PYB11property("std::vector<double>&", getter="getRadii")
