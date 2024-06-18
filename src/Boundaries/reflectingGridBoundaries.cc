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

    ReflectingGridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        GridBoundaries<dim>(grid,physics) {
        if (dim == 1) {
            std::vector<int> leftIds = grid->leftMost();  
            std::vector<int> rightIds = grid->rightMost();

            boundaryIds.push_back(leftIds);
            boundaryIds.push_back(rightIds);
        }
        else if (dim == 2) {
            std::vector<int> leftIds   = grid->leftMost();  
            std::vector<int> rightIds  = grid->rightMost();
            std::vector<int> topIds    = grid->topMost();
            std::vector<int> bottomIds = grid->bottomMost();

            boundaryIds.push_back(leftIds);
            boundaryIds.push_back(rightIds);
            boundaryIds.push_back(topIds);
            boundaryIds.push_back(bottomIds);
        }
        else if (dim == 3) {
            std::vector<int> leftIds   = grid->leftMost();  
            std::vector<int> rightIds  = grid->rightMost();
            std::vector<int> topIds    = grid->topMost();
            std::vector<int> bottomIds = grid->bottomMost();
            std::vector<int> frontIds  = grid->frontMost();
            std::vector<int> backIds   = grid->backMost();

            boundaryIds.push_back(leftIds);
            boundaryIds.push_back(rightIds);
            boundaryIds.push_back(topIds);
            boundaryIds.push_back(bottomIds);
            boundaryIds.push_back(frontIds);
            boundaryIds.push_back(backIds);
        }
    }
    
    virtual ~ReflectingGridBoundaries() {}

    virtual void
    ApplyBoundaries() override {
        Physics<dim>* physics = this->physics;
        State<dim>* state = physics->getState();
        for (int i = 0; i < state->count(); ++i) {
            FieldBase* field = state->getFieldByIndex(i); // Get the field at index i

            if (dynamic_cast<ScalarField*>(field) != nullptr) {
                ScalarField* doubleField = dynamic_cast<ScalarField*>(field);

                #pragma omp parallel for
                for (int j=0; j<boundaryIds.size();++j){
                    int i = boundaryIds[j];
                    doubleField->setValue(i,0);
                }
            
            } else if (dynamic_cast<VectorField*>(field) != nullptr) {
                VectorField* vectorField = dynamic_cast<VectorField*>(field);

                #pragma omp parallel for
                for (int j=0; j<boundaryIds.size();++j) {
                    int i = boundaryIds[j];
                    vectorField->setValue(i,Vector());
                }            
            }
        }
    }
};
