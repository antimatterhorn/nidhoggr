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
                if(thisPos[i] < p1[i] || thisPos[i] > p2[i])
                    inside = false;
            if(inside)
                ids.push_back(idx);
        }
    }

    virtual void
    ApplyBoundaries() override {
        Physics<dim>* physics = this->physics;
        for (FieldBase* field : physics->derivFields) {
            int numNodes = field->size();
            if (typeid(*field) == typeid(Field<double>)) {
                Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
                if (doubleField) {
                    #pragma omp parallel for
                    for (int j=0; j<ids.size();++j){
                        int i = ids[j];
                        doubleField->setValue(i,0);
                    }
                }
/*
This is not a safe way to do this. For one thing, when you eventually parallelize all these routines
it is not guaranteed that "i" is the correct id. You should eventually find a way to implement an id
lookup from the field->nodeList->getField("id")[i] or some such. But at the moment, I'm not sure how
to do that without getting into a circular dependence hell with the fields and nodeLists. Database??
*/
            } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
                Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                if (vectorField) {
                    #pragma omp parallel for
                        for (int j=0; j<ids.size();++j) {
                            int i = ids[j];
                            vectorField->setValue(i,Lin::Vector<dim>());
                        }
                }
            }
        }
    }
};
