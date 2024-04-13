from PYB11Generator import *

@PYB11template("typename T")
class FieldList:
    def pyinit(self,fieldName="std::string",numFields="int"):
        return
    def addField(self,field="%(typename T)s"):
        return
    @PYB11returnpolicy("reference_internal")
    @PYB11implementation("[](T& self, int i) { const int n = self.size(); if (i >= n) throw py::index_error(); return &self[(i %% n + n) %% n]; }")
    def __call__(self):
        "Index into a Field"
        #return "%(Value)s&"
        return
    
    size = PYB11property("int", getter="getSize", doc="The size of the FieldList.")