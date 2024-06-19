#ifndef DOMAINBOUNDARIES_HH
#define DOMAINBOUNDARIES_HH

#include <vector>
#include "../Mesh/grid.hh"
#include "../Physics/physics.hh"
#include "boundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class DomainBoundaries : public Boundaries<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> ids;
    std::vector<Mesh::Grid<dim>> bounds;
public:
    using Vector      = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    DomainBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        Boundaries<dim>(physics),
        grid(grid) {
        if (dim == 1) {
            double dx = grid->dx;
            Mesh::Grid<dim> leftGrid(1,dx);
            Mesh::Grid<dim> rightGrid(1,dx);
            bounds.push_back(leftGrid);
            bounds.push_back(rightGrid);
        }
        if (dim == 2) {
            int nx = grid->size_x();
            int ny = grid->size_y();
            double dx = grid->dx;
            double dy = grid->dy;
            Mesh::Grid<dim> leftGrid(1,ny,dx,dy);
            Mesh::Grid<dim> rightGrid(1,ny,dx,dy);
            Mesh::Grid<dim> topGrid(nx,1,dx,dy);
            Mesh::Grid<dim> bottomGrid(nx,1,dx,dy);
            bounds.push_back(leftGrid);
            bounds.push_back(rightGrid);
            bounds.push_back(topGrid);
            bounds.push_back(bottomGrid);
        }
        if (dim == 2) {
            int nx = grid->size_x();
            int ny = grid->size_y();
            int nz = grid->size_z();
            double dx = grid->dx;
            double dy = grid->dy;
            double dz = grid->dz;
            Mesh::Grid<dim> leftGrid(1,ny,nz,dx,dy,dz);
            Mesh::Grid<dim> rightGrid(1,ny,nz,dx,dy,dz);
            Mesh::Grid<dim> topGrid(nx,1,nz,dx,dy,nz);
            Mesh::Grid<dim> bottomGrid(nx,1,nz,dx,dy,dz);
            Mesh::Grid<dim> frontGrid(nx,ny,1,dx,dy,nz);
            Mesh::Grid<dim> backGrid(nx,ny,1,dx,dy,dz);
            bounds.push_back(leftGrid);
            bounds.push_back(rightGrid);
            bounds.push_back(topGrid);
            bounds.push_back(bottomGrid);
            bounds.push_back(frontGrid);
            bounds.push_back(backGrid);
        }
        insertFields(physics);
    }
    
    virtual ~DomainBoundaries() {}

    virtual std::vector<Mesh::Grid<dim>>* 
    getBounds() const { return *bounds;}

    virtual void
    insertFields(Physics<dim>* physics) {
        State<dim>* state = physics->getState();
        for (int i = 0; i < state->count(); ++i) {
            FieldBase* field = state->getFieldByIndex(i); // Get the field at index i
            if (typeid(*field) == typeid(ScalarField)) {
                for (int j = 0; j<bounds.size(); ++j) {
                    bounds[j].template insertField<double>(field->getNameString);
                }
            }
            else if (typeid(*field) == typeid(VectorField)) {
                for (int j = 0; j<bounds.size(); ++j) {
                    bounds[j].template insertField<Vector>(field->getNameString);
                }
            }
        }
    }

};

#endif // DOMAINBOUNDARIES_HH