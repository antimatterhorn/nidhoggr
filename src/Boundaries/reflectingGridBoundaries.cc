#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class ReflectingGridBoundaries : public GridBoundaries<dim> {
protected:
    std::vector<std::vector<int>> boundaryIds;
public:
    using Vector=Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    ReflectingGridBoundaries(Mesh::Grid<dim>* grid) : 
        GridBoundaries<dim>(grid) {
        
    }
    
    virtual ~ReflectingGridBoundaries() {}

    virtual void
    ApplyBoundaries(State<dim>& state, NodeList* nodeList) override {

    }
};
