#include "physics.hh"
#include "../Mesh/femesh.hh"
#include <iostream>

template <int dim>
class FEM : public Physics<dim> {
protected:
    Mesh::FEMesh<dim>* mesh;
    double dtmin;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    FEM(NodeList* nodeList, PhysicalConstants& constants, Mesh::FEMesh<dim>* mesh) : 
        Physics<dim>(nodeList,constants),
        mesh(mesh) {
    }

    ~FEM() { delete mesh; }
};