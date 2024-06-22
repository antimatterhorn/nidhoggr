#include <vector>
#include "../Physics/physics.hh"
#include "../DataBase/nodeList.hh"
#include "collider.hh"

// Base class for Grid Boundaries
template <int dim>
class BoxCollider : public Collider<dim> {
protected:
    NodeList* nodeList;
    Lin::Vector<dim> position1;
    Lin::Vector<dim> position2;
    double elasticity;

public:
    using Vector=Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    BoxCollider(Physics<dim>* physics, Vector& position1, Vector& position2) : 
        Collider<dim>(physics),
        position1(position1), position2(position2){
        nodeList = physics->getNodeList();
        elasticity = 1.0;
    }
    
    BoxCollider(Physics<dim>* physics, Vector& position, Vector& position2, double elasticity) : 
        Collider<dim>(physics),
        position1(position1), position2(position2), elasticity(elasticity) {
        nodeList = physics->getNodeList();
        if (elasticity > 1.0)
            std::cerr << "elasticity > 1. was this intentional?" << std::endl;
    }

    virtual ~BoxCollider() {}

    virtual void
    ApplyBoundaries() override {  
        Physics<dim>* physics   = this->physics;
        State<dim>* state       = physics->getState();
        int numNodes            = state->size();
        VectorField* positions  = state->template getField<Vector>("position");
        VectorField* velocities = state->template getField<Vector>("velocity");
        ScalarField* radii      = nodeList->getField<double>("radius");

    }

    bool 
    Inside(Lin::Vector<dim>& otherPosition, double otherRadius) override {
        bool insideX,insideY=true,insideZ=true;
        
        // Determine the minimum and maximum coordinates for the box
        float minX = std::min(position1.x(), position2.x());
        float maxX = std::max(position1.x(), position2.x());
        insideX = (otherPosition.x() - otherRadius >= minX) && (otherPosition.x() + otherRadius <= maxX);
        if (dim > 1) {
            float minY = std::min(position1.y(), position2.y());
            float maxY = std::max(position1.y(), position2.y());
            insideY = (otherPosition.y() - otherRadius >= minY) && (otherPosition.y() + otherRadius <= maxY);
        }
        if (dim > 2) {
            float minZ = std::min(position1.z(), position2.z());
            float maxZ = std::max(position1.z(), position2.z());
            insideZ = (otherPosition.z() - otherRadius >= minZ) && (otherPosition.z() + otherRadius <= maxZ);
        }
            
        
        // The particle is inside the box if it satisfies all three dimensions
        return insideX && insideY && insideZ;
    }

};