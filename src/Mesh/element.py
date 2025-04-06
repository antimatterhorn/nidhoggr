from PYB11Generator import *


class TriangleElement:
    def pyinit(self,nodeIndices="std::vector<size_t>"):
        return
    def computeArea(self,positions="std::vector<Lin::Vector<2>>"):
        return "double"
    
class QuadElement:
    def pyinit(self,nodeIndices="std::vector<size_t>"):
        return
    def computeArea(self,positions="std::vector<Lin::Vector<2>>"):
        return "double"



# Expose the ElementType enum from C++
ElementType = PYB11enum(
    name="ElementType",
    values=[
        ("Line",        "Mesh::ElementType::Line"),
        ("Triangle",    "Mesh::ElementType::Triangle"),
        ("Quad",        "Mesh::ElementType::Quad"),
        ("Tetrahedron", "Mesh::ElementType::Tetrahedron"),
        ("Hexahedron",  "Mesh::ElementType::Hexahedron"),
        # Add more types here if needed
    ],
    namespace="Mesh"
)

print("DEBUG ElementType is a", type(ElementType))
print("DEBUG ElementType =", ElementType)
print("ElementType enum values:", ElementType.values)