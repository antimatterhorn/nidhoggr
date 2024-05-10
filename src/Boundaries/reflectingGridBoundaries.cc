#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class ReflectingGridBoundaries : public GridBoundaries<dim> {
protected:

public:
    ReflectingGridBoundaries(Mesh::Grid<dim>* grid) : 
        GridBoundaries<dim>(grid) {}
    
    virtual ~ReflectingGridBoundaries() {}

};
