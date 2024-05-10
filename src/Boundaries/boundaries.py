from PYB11Generator import *

@PYB11template("dim")
class Boundaries:
    def pyinit(self,physics="Physics<%(dim)s>*"):
        return