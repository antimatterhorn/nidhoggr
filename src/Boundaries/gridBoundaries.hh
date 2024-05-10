#ifndef GRIDBOUNDARIES_HH
#define GRIDBOUNDARIES_HH

#include <vector>
#include "../Mesh/grid.hh"
#include "../Physics/physics.hh"
#include "boundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class GridBoundaries : public Boundaries<dim> {
protected:
    Mesh::Grid<dim>* grid;
public:
    GridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        Boundaries<dim>(physics),
        grid(grid) {}
    
    virtual ~GridBoundaries() {}

};

#endif // GRIDBOUNDARIES_HH