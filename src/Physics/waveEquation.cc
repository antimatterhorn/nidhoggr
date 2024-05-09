#include "physics.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class WaveEquation : public Physics<dim> {
protected:
    Mesh::Grid<dim> grid;
public:
    WaveEquation() {}

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>& grid) : 
        Physics<dim>(nodeList,constants),
        grid(grid) {

        int numNodes = nodeList->size();

    }

    ~WaveEquation() {}

};
