#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "../DataBase/nodeList.hh"
#include "../DataBase/dataBase.hh"
#include "../Math/vectorMath.hh"
#include "../Type/physicalConstants.hh"

template <int dim>
class Physics {
protected:
    NodeList* nodeList;
    PhysicalConstants& constants;
public:
    std::vector<FieldBase*> derivFields;
    
    Physics(NodeList* nodeList, PhysicalConstants& constants) : 
        nodeList(nodeList), 
        constants(constants) {
        VerifyFields(nodeList);
    }

    virtual ~Physics() {}

    virtual void
    EvaluateDerivatives(const Field<double>* initialState, Field<double>& deriv, const double t)  {  }

    virtual void
    EvaluateDerivatives(const Field<Lin::Vector<dim>>* initialState, Field<Lin::Vector<dim>>& deriv, const double t) {  }

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

    virtual double 
    EstimateTimestep() const { return 0; }

    virtual NodeList*
    getNodeList() const { return nodeList; }
};

#endif //PHYSICS_HH