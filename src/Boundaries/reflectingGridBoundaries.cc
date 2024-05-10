#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class ReflectingGridBoundaries : public GridBoundaries<dim> {
protected:

public:
    ReflectingGridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        GridBoundaries<dim>(grid,physics) {}
    
    virtual ~ReflectingGridBoundaries() {}

};
