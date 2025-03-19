#include <vector>
#include "gridBoundaries.hh"
#include "../Math/vectorMath.hh"

// Base class for Grid Boundaries
template <int dim>
class DirichletGridBoundaries : public GridBoundaries<dim> {
protected:
    std::vector<int> ids;
    Mesh::Grid<dim>* grid;

    void
    addIds(std::vector<int> vec) {
        for (int i=0; i<vec.size();i++) {
            ids.push_back(vec[i]);
        }
    }
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
    addDomain() {
        if (dim == 1) {
            std::vector<int> leftIds = grid->leftMost();  
            std::vector<int> rightIds = grid->rightMost();

            addIds(leftIds);
            addIds(rightIds);
        }
        else if (dim == 2) {
            std::vector<int> leftIds   = grid->leftMost();  
            std::vector<int> rightIds  = grid->rightMost();
            std::vector<int> topIds    = grid->topMost();
            std::vector<int> bottomIds = grid->bottomMost();

            addIds(leftIds);
            addIds(rightIds);
            addIds(topIds);
            addIds(bottomIds);
        }
        else if (dim == 3) {
            std::vector<int> leftIds   = grid->leftMost();  
            std::vector<int> rightIds  = grid->rightMost();
            std::vector<int> topIds    = grid->topMost();
            std::vector<int> bottomIds = grid->bottomMost();
            std::vector<int> frontIds  = grid->frontMost();
            std::vector<int> backIds   = grid->backMost();

            addIds(leftIds);
            addIds(rightIds);
            addIds(topIds);
            addIds(bottomIds);
            addIds(frontIds);
            addIds(backIds);
        }
    }

    virtual void
    ApplyBoundaries() override {
        Physics<dim>* physics = this->physics;
        State<dim>* state = physics->getState();
        NodeList* nodeList = physics->getNodeList();
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
