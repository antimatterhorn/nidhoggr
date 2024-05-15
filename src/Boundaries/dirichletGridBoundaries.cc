#include <vector>
#include "gridBoundaries.hh"
#include "../Math/vectorMath.hh"

// Base class for Grid Boundaries
template <int dim>
class DirichletGridBoundaries : public GridBoundaries<dim> {
protected:
    std::vector<int> ids;
    Mesh::Grid<dim>* grid;
public:
    DirichletGridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        GridBoundaries<dim>(grid,physics),
        grid(grid) {}
    
    virtual ~DirichletGridBoundaries() {}

    virtual void
    addBox(Lin::Vector<dim> p1, Lin::Vector<dim> p2){
        #pragma omp parallel for
        for (int idx = 0; idx<grid->size(); ++idx) {
            Lin::Vector<dim> thisPos = grid->getPosition(idx);
            bool inside = true;
            for(int i=0;i<dim;++i)
                if(thisPos[i] < p1[i] || thisPos[i] > p2[i]) {
                    inside = false;
                    break;
                }
            if(inside)
                ids.push_back(idx);
        }
    }

    virtual void 
    removeBox(Lin::Vector<dim> p1, Lin::Vector<dim> p2) {
        #pragma omp parallel for
        for (int idx = 0; idx < grid->size(); ++idx) {
            Lin::Vector<dim> thisPos = grid->getPosition(idx);
            bool inside = true;
            for (int i = 0; i < dim; ++i) {
                if (thisPos[i] < p1[i] || thisPos[i] > p2[i]) {
                    inside = false;
                    break;
                }
            }
            if (inside) {
                // Check if idx is in the ids vector
                auto it = std::find(ids.begin(), ids.end(), idx);
                if (it != ids.end()) {
                    // Element found, remove it
                    #pragma omp critical
                    ids.erase(it);
                }
            }
        }
    }

    virtual void
    ApplyBoundaries() override {
        Physics<dim>* physics = this->physics;
        State<dim>* state = physics->getState();
        for (int i = 0; i < state->count(); ++i) {
            FieldBase* field = state->getFieldByIndex(i); // Get the field at index i

            if (dynamic_cast<Field<double>*>(field) != nullptr) {
                Field<double>* doubleField = dynamic_cast<Field<double>*>(field);

                #pragma omp parallel for
                for (int j=0; j<ids.size();++j){
                    int i = ids[j];
                    doubleField->setValue(i,0);
                }
            
            } else if (dynamic_cast<Field<Lin::Vector<dim>>*>(field) != nullptr) {
                Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);

                #pragma omp parallel for
                for (int j=0; j<ids.size();++j) {
                    int i = ids[j];
                    vectorField->setValue(i,Lin::Vector<dim>());
                }
            
            }
        }
    }
};
