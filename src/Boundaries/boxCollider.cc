#include <vector>
#include "../Physics/physics.hh"
#include "../DataBase/nodeList.hh"
#include "collider.hh"
#include <algorithm>

// Base class for Grid Boundaries
template <int dim>
class BoxCollider : public Collider<dim> {
protected:
    NodeList* nodeList;
    Lin::Vector<dim> pos1;
    Lin::Vector<dim> pos2;
    double elasticity;
    Lin::Vector<dim> minima;
    Lin::Vector<dim> maxima;


public:
    using Vector=Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    BoxCollider(Physics<dim>* physics, Vector& position1, Vector& position2) : 
        Collider<dim>(physics),
        pos1(position1), pos2(position2){
        nodeList = physics->getNodeList();
        elasticity = 1.0;
        findExtrema(); // this protects against foolishness
    }
    
    BoxCollider(Physics<dim>* physics, Vector& position1, Vector& position2, double elasticity) : 
        Collider<dim>(physics),
        pos1(position1), pos2(position2), elasticity(elasticity) {
        nodeList = physics->getNodeList();
        if (elasticity > 1.0)
            std::cerr << "elasticity > 1. was this intentional?" << std::endl;
        findExtrema();
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
        
        if (radii!= nullptr) {
            for (int i=0; i<numNodes; ++i) {
                Vector pos = positions->getValue(i);
                double rad = radii->getValue(i);
                if (Inside(pos,rad)) {
                    Vector v1 = velocities->getValue(i);
                    Vector n  = faceNormal(pos);
                    Vector v2 = v1 - 2.0*(v1*n)*n;
                    velocities->setValue(i,v2*elasticity);                 // reflect across the normal
                    Vector dp = physics->lastStep()*velocities->getValue(i);
                    Vector newPos = pos + (0.5*dp);  // move it out of the boundary by a half-step
                    positions->setValue(i,newPos);
                }
            }
            physics->PushState(state);
        }
        else
        {
            std::cerr << "radius not found in nodeList. was this intentional?" << std::endl;
        }
    }

    bool 
    Inside(Lin::Vector<dim>& otherPosition, double otherRadius) override {
        bool insideX,insideY=true,insideZ=true;

        insideX = (otherPosition.x() - otherRadius >= minima.x()) && (otherPosition.x() + otherRadius <= maxima.x());
        if (dim > 1)
            insideY = (otherPosition.y() - otherRadius >= minima.y()) && (otherPosition.y() + otherRadius <= maxima.y());
        if (dim > 2)
            insideZ = (otherPosition.z() - otherRadius >= minima.z()) && (otherPosition.z() + otherRadius <= maxima.z());
        
        // The particle is inside the box if it satisfies all three dimensions
        return insideX && insideY && insideZ;
    }

    double distanceToEdge(Vector point, Vector A, Vector B) {
        double t = (point-A)*(B-A)/((B-A)*(B-A));
        if (t<=0)
            return (point-A).magnitude();
        else if (t>=1)
            return (point-B).magnitude();
        else {
            Vector q = A + t*(B-A);
            return (point-q).magnitude();
        }
    }

    Vector
    faceNormal(Vector point) {
        if (dim == 1) {
            return ((point.x() - minima.x()) < (point.x() - maxima.x()) ? Vector{-1} : Vector{1});
        }
        if (dim == 2) {
            double d1 = distanceToEdge(point,minima,Vector{minima.x(),maxima.y()});
            double d2 = distanceToEdge(point,Vector{minima.x(),maxima.y()},maxima);
            double d3 = distanceToEdge(point,maxima,Vector{maxima.x(),minima.y()});
            double d4 = distanceToEdge(point,Vector{maxima.x(),minima.y()},minima);
            double dmin = std::min({d1,d2,d3,d4});
            if (dmin==d1)
                return Vector{-1,0};
            else if (dmin==d2)
                return Vector{0,1};
            else if (dmin==d3)
                return Vector{1,0};
            else
                return Vector{0,-1};
        }
        if (dim == 3) {
            // do something!
            return Vector{0,1,0};
        }
    }

    void
    findExtrema() {
        if (dim==1) {
            double minX = std::min(pos1.x(), pos2.x());
            double maxX = std::max(pos1.x(), pos2.x());
            minima = Vector{minX};
            maxima = Vector{maxX};
        }
        if (dim==2) {
            double minX = std::min(pos1.x(), pos2.x());
            double maxX = std::max(pos1.x(), pos2.x());
            double minY = std::min(pos1.y(), pos2.y());
            double maxY = std::max(pos1.y(), pos2.y());
            minima = Vector{minX,minY};
            maxima = Vector{maxX,maxY};
        }
        if (dim > 2) {
            double minX = std::min(pos1.x(), pos2.x());
            double maxX = std::max(pos1.x(), pos2.x());
            double minY = std::min(pos1.y(), pos2.y());
            double maxY = std::max(pos1.y(), pos2.y());
            double minZ = std::min(pos1.z(), pos2.z());
            double maxZ = std::max(pos1.z(), pos2.z());
            minima = Vector{minX,minY,minZ};
            maxima = Vector{maxX,maxY,maxZ};
        }
    }

};