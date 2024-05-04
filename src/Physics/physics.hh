#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "../DataBase/nodeList.hh"
#include "../Math/vectorMath.hh"

namespace Physics {
template <int dim>
class Physics {
protected:
    NodeList* nodeList;
public:
    Field<double> mass; // Pointer to Field<double>

    Physics(NodeList* nodeListPtr) : nodeList(nodeListPtr) {
        int numNodes = nodeList->size();
        if (nodeList->mass() == nullptr) {
            mass = new Field<double>("mass", numNodes); // Create a new Field<double>
            nodeList->addField(&mass); // Add the mass field to the nodeList
        } else {
            mass = *nodeList->mass(); // Assign mass to point to the existing mass field
        } 
    }

    virtual ~Physics() {

    }
};
}

#endif //PHYSICS_HH