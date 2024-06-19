#include <vector>
#include "domainBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class PacmanDomainBoundaries : public DomainBoundaries<dim> {
protected:

public:
    using Vector      = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;
    
    PacmanDomainBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        DomainBoundaries<dim>(grid,physics) {}
    
    virtual ~PacmanDomainBoundaries() {}

    virtual void
    ApplyBoundaries() override {

    }

};
