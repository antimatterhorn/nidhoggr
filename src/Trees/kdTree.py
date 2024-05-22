from PYB11Generator import *

@PYB11template("dim")
class Physics:
    def pyinit(self,
               points="Lin::Vector<%(dim)s>&"):
        return