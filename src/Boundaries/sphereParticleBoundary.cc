#include <vector>
#include "../Physics/physics.hh"
#include "../DataBase/nodeList.hh"
#include "boundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class SphereParticleBoundary : public Boundaries<dim> {
protected:
    NodeList* nodeList;
    Lin::Vector<dim> position;
    double radius;

    bool Inside(Lin::Vector<dim>& otherPosition, double otherRadius)
    {
        double distance = (position-otherPosition).magnitude();
        if (distance <= (radius + otherRadius))
            return true;
        else
            return false;
    }
public:
    using Vector=Lin::Vector<dim>;

    SphereParticleBoundary(Physics<dim>* physics, Vector& position, double radius) : 
        Boundaries<dim>(physics),
        position(position), radius(radius) {
        nodeList = physics->getNodeList();
    }
    
    virtual ~SphereParticleBoundary() {}

    virtual void
    ApplyBoundaries() override {  

    }
    

};