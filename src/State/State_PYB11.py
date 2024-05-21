from PYB11Generator import *
PYB11includes = ['"state.hh"']

@PYB11template("dim")
class State:
    def pyinit(self):
        return
    def pyinit1(self,numNodes="int"):
        return