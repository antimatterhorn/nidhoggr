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

    bool 
    Inside(Lin::Vector<dim>& otherPosition, double otherRadius) {
        double distance = (position-otherPosition).magnitude();
        if (distance <= (radius + otherRadius))
            return true;
        else
            return false;
    }

    Lin::Vector<dim>
    normal(Lin::Vector<dim>& otherPosition) {
        return (otherPosition-position).normal();
    }
public:
    using Vector=Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    SphereParticleBoundary(Physics<dim>* physics, Vector& position, double radius) : 
        Boundaries<dim>(physics),
        position(position), radius(radius) {
        nodeList = physics->getNodeList();
    }
    
    virtual ~SphereParticleBoundary() {}

    virtual void
    ApplyBoundaries() override {  
        Physics<dim>* physics   = this->physics;
        State<dim>* state       = physics->getState();
        int numNodes            = state->size();
        VectorField* positions  = state->template getField<Vector>("position");
        VectorField* velocities = state->template getField<Vector>("velocity");
        ScalarField* radii      = nodeList->getField<double>("radius");

        if (radii!= nullptr) {
            for (int i=0; i<numNodes; ++i) {
                Vector pos = positions->getValue(i);
                double rad = radii->getValue(i);
                if (Inside(pos,rad)) {
                    Vector v1 = velocities->getValue(i);
                    Vector n  = (pos-position).normal();
                    Vector v2 = v1 - 2.0*(v1*n)*n;
                    velocities->setValue(i,v2);                 // reflect across the normal

                    Vector newPos = position + (rad+radius)*n;  // move it out of the boundary
                    positions->setValue(i,newPos);
                }
            }
        }
        else
        {
            std::cerr << "radius not found in nodeList. was this intentional?" << std::endl;
        }
    }

};