from PYB11Generator import *

@PYB11template("typename T")
class FieldList:
    def pyinit(self,field="Field<%(typename T)s>",numFields="int"):
        return
    
    @PYB11returnpolicy("reference_internal")
    @PYB11implementation("[](T& self, int i) { const int n = self.size(); if (i >= n) throw py::index_error(); return &self[(i %% n + n) %% n]; }")
    def __call__(self):
        "Index into a Field"
        #return "%(Value)s&"
        return