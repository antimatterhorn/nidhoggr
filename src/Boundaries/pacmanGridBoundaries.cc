#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class PacmanGridBoundaries : public GridBoundaries<dim> {
protected:

public:
    using Vector      = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;
    
    PacmanGridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        GridBoundaries<dim>(grid,physics) {}
    
    virtual ~PacmanGridBoundaries() {}

    virtual void
    ApplyBoundaries() override {
        Mesh::Grid<dim>* grid = this->grid;
        Physics<dim>* physics = this->physics;
        State<dim>* state = physics->getState();
        for (int i = 0; i < state->count(); ++i) {
            FieldBase* field = state->getFieldByIndex(i); // Get the field at index i
            if (typeid(*field) == typeid(ScalarField)) {
                ScalarField* doubleField = dynamic_cast<ScalarField*>(field);
                if (doubleField) {
                    std::vector<int> leftIds  = grid->leftMost();
                    std::vector<int> rightIds = grid->rightMost();
                    ScalarField temp  = ScalarField("temp",leftIds.size());
                    SwapFieldData(doubleField,leftIds,rightIds);
                    
                    if constexpr (dim > 1) {
                        std::vector<int> topIds    = grid->topMost();
                        std::vector<int> bottomIds = grid->bottomMost();
                        ScalarField temp    = ScalarField("temp",topIds.size());
                        SwapFieldData(doubleField,topIds,bottomIds);
                    }
                    if constexpr (dim > 2) {
                        std::vector<int> frontIds = grid->frontMost();
                        std::vector<int> backIds  = grid->backMost();
                        ScalarField temp = ScalarField("temp",frontIds.size());
                        SwapFieldData(doubleField,frontIds,backIds);
                    }
                }
            } else if (typeid(*field) == typeid(VectorField)) {
                VectorField* vectorField = dynamic_cast<VectorField*>(field);
                if (vectorField) {
                    std::vector<int> leftIds  = grid->leftMost();
                    std::vector<int> rightIds = grid->rightMost();
                    VectorField temp  = VectorField("temp",leftIds.size());
                    SwapFieldData(vectorField,leftIds,rightIds);
                    
                    if constexpr (dim > 1) {
                        std::vector<int> topIds    = grid->topMost();
                        std::vector<int> bottomIds = grid->bottomMost();
                        VectorField temp    = VectorField("temp",topIds.size());
                        SwapFieldData(vectorField,topIds,bottomIds);
                    }
                    if constexpr (dim > 2) {
                        std::vector<int> frontIds = grid->frontMost();
                        std::vector<int> backIds  = grid->backMost();
                        VectorField temp = VectorField("temp",frontIds.size());
                        SwapFieldData(vectorField,frontIds,backIds);
                    }
                }
            }
        }
    }

    virtual void
    SwapFieldData(ScalarField* field,
                    std::vector<int>& ids1,
                    std::vector<int>& ids2) {
        for (int i = 0; i<ids1.size(); ++i) {
            int id1 = ids1[i];
            int id2 = ids2[i];
            double temp = field->getValue(id1);
            field->setValue(id1,field->getValue(id2));
            field->setValue(id2,temp);
        }
    }

    virtual void
    SwapFieldData(VectorField* field, 
                    std::vector<int>& ids1,
                    std::vector<int>& ids2) {
        for (int i = 0; i<ids1.size(); ++i) {
            int id1 = ids1[i];
            int id2 = ids2[i]; 
            Vector temp = field->getValue(id2);
            field->setValue(id1,field->getValue(id2));
            field->setValue(id2,temp);
        }
    }
};
