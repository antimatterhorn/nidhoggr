#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "../DataBase/nodeList.hh"
#include "../DataBase/dataBase.hh"
#include "../Math/vectorMath.hh"
#include "../Type/physicalConstants.hh"
#include "../Integrator/integrator.hh"

template <int dim>
class Physics {
protected:
    NodeList* nodeList;
    PhysicalConstants& constants;
public:
    Physics(NodeList* _nodeList, PhysicalConstants& _constants) : 
        nodeList(_nodeList), 
        constants(_constants) {
        VerifyFields(nodeList);
    }

    virtual ~Physics() {}

    virtual void
    EvaluateDerivatives(const double dt) {}

    virtual void
    VerifyFields(NodeList* nodeList) {
        int numNodes = nodeList->size();
        if (nodeList->mass() == nullptr)
            nodeList->insertField<double>("mass"); // Add the mass field to the nodeList

        if (nodeList->velocity<dim>() == nullptr) 
            nodeList->insertField<Lin::Vector<dim>>("velocity"); // Add the velocity field to the nodeList

        if (nodeList->position<dim>() == nullptr) 
            nodeList->insertField<Lin::Vector<dim>>("position"); // Add the position field to the nodeList
    }
};

#endif //PHYSICS_HH