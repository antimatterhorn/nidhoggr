#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "../DataBase/nodeList.hh"
#include "../Math/vectorMath.hh"
#include "../Type/physicalConstants.hh"
#include "../Integrator/integrator.hh"

namespace Physics {
template <int dim>
class Physics {
protected:
    NodeList* nodeList;
public:
    Field<double> mass; // Pointer to Field<double>
    Field<Lin::Vector<dim>> position;
    Field<Lin::Vector<dim>> velocity;
    PhysicalConstants& constants;

    Physics(NodeList* nodeListPtr, PhysicalConstants& constants) : 
        nodeList(nodeListPtr), 
        constants(constants) {
        int numNodes = nodeList->size();
        if (nodeList->mass() == nullptr) {
            mass = new Field<double>("mass", numNodes); // Create a new Field<double>
            nodeList->addField(&mass); // Add the mass field to the nodeList
        } else {
            mass = *nodeList->mass(); // Assign mass to point to the existing mass field
        } 

        if (nodeList->velocity<dim>() == nullptr) {
            velocity = new Field<Lin::Vector<dim>>("velocity", numNodes); // Create a new Field<double>
            nodeList->addField(&velocity); // Add the mass field to the nodeList
        } else {
            velocity = *nodeList->velocity<dim>(); // Assign mass to point to the existing mass field
        }

        if (nodeList->position<dim>() == nullptr) {
            position = new Field<Lin::Vector<dim>>("position", numNodes); // Create a new Field<double>
            nodeList->addField(&position); // Add the mass field to the nodeList
        } else {
            position = *nodeList->position<dim>(); // Assign mass to point to the existing mass field
        }
    }

    virtual ~Physics() {}

    virtual void
    UpdateDerivatives(const double dt) const = 0;
};
using Physics1D = Physics<1>;
using Physics2D = Physics<2>;
using Physics3D = Physics<3>;
}

#endif //PHYSICS_HH