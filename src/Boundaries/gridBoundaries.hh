#ifndef GRIDBOUNDARIES_HH
#define GRIDBOUNDARIES_HH

#include <vector>
#include "../Mesh/grid.hh"

// Base class for Grid Boundaries
template <int dim>
class GridBoundaries {
protected:
    Mesh::Grid<dim>* grid;
public:
    GridBoundaries(Mesh::Grid<dim>* grid) : grid(grid) {}
    
    virtual ~GridBoundaries() {}

    virtual void
    ApplyBoundaries() {}

};

#endif // GRIDBOUNDARIES_HH