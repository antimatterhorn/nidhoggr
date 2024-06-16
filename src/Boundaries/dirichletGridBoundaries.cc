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
    using Vector      = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    DirichletGridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        GridBoundaries<dim>(grid,physics),
        grid(grid) {}
    
    virtual ~DirichletGridBoundaries() {}

    virtual void
    addBox(Vector p1, Vector p2){
        #pragma omp parallel for
        for (int idx = 0; idx<grid->size(); ++idx) {
            Vector thisPos = grid->getPosition(idx);
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
    removeBox(Vector p1, Vector p2) {
        #pragma omp parallel for
        for (int idx = 0; idx < grid->size(); ++idx) {
            Vector thisPos = grid->getPosition(idx);
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
    addSphere(Vector p, double radius){
        #pragma omp parallel for
        for (int idx = 0; idx<grid->size(); ++idx) {
            Vector thisPos = grid->getPosition(idx);
            bool inside = true;
            if ((thisPos - p).mag2() <= radius*radius)
                ids.push_back(idx);
        }
    }

    virtual void
    removeSphere(Vector p, double radius){
        #pragma omp parallel for
        for (int idx = 0; idx<grid->size(); ++idx) {
            Vector thisPos = grid->getPosition(idx);
            bool inside = true;
            if ((thisPos - p).mag2() <= radius*radius) {
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

            if (dynamic_cast<ScalarField*>(field) != nullptr) {
                ScalarField* doubleField = dynamic_cast<ScalarField*>(field);

                #pragma omp parallel for
                for (int j=0; j<ids.size();++j){
                    int i = ids[j];
                    doubleField->setValue(i,0);
                }
            
            } else if (dynamic_cast<VectorField*>(field) != nullptr) {
                VectorField* vectorField = dynamic_cast<VectorField*>(field);

                #pragma omp parallel for
                for (int j=0; j<ids.size();++j) {
                    int i = ids[j];
                    vectorField->setValue(i,Vector());
                }
            
            }
        }
    }
};
