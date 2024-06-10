#include <vector>
#include "../Physics/physics.hh"
#include "boundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class SphereParticleBoundary : public Boundaries<dim> {
protected:
    Mesh::Grid<dim>* grid;
public:
    SphereParticleBoundary(Physics<dim>* physics) : 
        Boundaries<dim>(physics),
        grid(grid) {}
    
    virtual ~SphereParticleBoundary() {}

    virtual void
    ApplyBoundaries() override {  }
    

};