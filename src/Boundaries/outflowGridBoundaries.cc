#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class OutflowGridBoundaries : public GridBoundaries<dim> {
protected:

public:
    OutflowGridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        GridBoundaries<dim>(grid,physics) {}
    
    virtual ~OutflowGridBoundaries() {}

    virtual void
    ApplyBoundaries() override {
        Mesh::Grid<dim>* grid = this->grid;
        Physics<dim>* physics = this->physics;
        State<dim>* state = physics->getState();
        for (int i = 0; i < state->count(); ++i) {
            FieldBase* field = state->getFieldByIndex(i); // Get the field at index i
            if (typeid(*field) == typeid(Field<double>)) {
                Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
                if (doubleField) {
                    ApplyOutflowBoundary(doubleField, grid);
                }
            } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
                Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                if (vectorField) {
                    ApplyOutflowBoundary(vectorField, grid);
                }
            }
        }
    }

    void ApplyOutflowBoundary(Field<double>* field, Mesh::Grid<dim>* grid) {
        // Handle left and right boundaries
        std::vector<int> leftIds = grid->leftMost();
        std::vector<int> rightIds = grid->rightMost();
        CopyBoundaryData(field, leftIds, 1); // Copy from cell adjacent to left boundary
        CopyBoundaryData(field, rightIds, -1); // Copy from cell adjacent to right boundary

        if constexpr (dim >= 2) {
            // Handle top and bottom boundaries
            std::vector<int> topIds = grid->topMost();
            std::vector<int> bottomIds = grid->bottomMost();
            CopyBoundaryData(field, topIds, -grid->size_x()); // Copy from cell adjacent to top boundary
            CopyBoundaryData(field, bottomIds, grid->size_x()); // Copy from cell adjacent to bottom boundary
        }

        if constexpr (dim == 3) {
            // Handle front and back boundaries
            std::vector<int> frontIds = grid->frontMost();
            std::vector<int> backIds = grid->backMost();
            CopyBoundaryData(field, frontIds, -grid->size_x() * grid->size_y()); // Copy from cell adjacent to front boundary
            CopyBoundaryData(field, backIds, grid->size_x() * grid->size_y()); // Copy from cell adjacent to back boundary
        }
    }

    void ApplyOutflowBoundary(Field<Lin::Vector<dim>>* field, Mesh::Grid<dim>* grid) {
        // Handle left and right boundaries
        std::vector<int> leftIds = grid->leftMost();
        std::vector<int> rightIds = grid->rightMost();
        CopyBoundaryData(field, leftIds, 1); // Copy from cell adjacent to left boundary
        CopyBoundaryData(field, rightIds, -1); // Copy from cell adjacent to right boundary

        if constexpr (dim >= 2) {
            // Handle top and bottom boundaries
            std::vector<int> topIds = grid->topMost();
            std::vector<int> bottomIds = grid->bottomMost();
            CopyBoundaryData(field, topIds, -grid->size_x()); // Copy from cell adjacent to top boundary
            CopyBoundaryData(field, bottomIds, grid->size_x()); // Copy from cell adjacent to bottom boundary
        }

        if constexpr (dim == 3) {
            // Handle front and back boundaries
            std::vector<int> frontIds = grid->frontMost();
            std::vector<int> backIds = grid->backMost();
            CopyBoundaryData(field, frontIds, -grid->size_x() * grid->size_y()); // Copy from cell adjacent to front boundary
            CopyBoundaryData(field, backIds, grid->size_x() * grid->size_y()); // Copy from cell adjacent to back boundary
        }
    }

    void CopyBoundaryData(Field<double>* field, const std::vector<int>& boundaryIds, int offset) {
        for (int id : boundaryIds) {
            field->setValue(id, field->getValue(id + offset));
        }
    }

    void CopyBoundaryData(Field<Lin::Vector<dim>>* field, const std::vector<int>& boundaryIds, int offset) {
        for (int id : boundaryIds) {
            field->setValue(id, field->getValue(id + offset));
        }
    }
};

