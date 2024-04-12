from PYB11Generator import *

@PYB11template("typename T")
class Field:
    def pyinit(self,name="std::string",value="%(typename T)s"):
        return

    name = PYB11property("std::string", getter="getName", doc="The name of the field.")
    fieldtype = PYB11property("std::string", getter="getType", doc="The datatype of the field.")

@PYB11template("typename T")
class FieldList:
    def pyinit(self,field="Field<%(typename T)s>",numFields="int"):
        return