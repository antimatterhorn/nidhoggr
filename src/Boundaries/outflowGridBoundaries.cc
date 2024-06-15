#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class OutflowGridBoundaries : public GridBoundaries<dim> {
protected:
    std::vector<std::vector<int>> boundaryIds;
public:
    OutflowGridBoundaries(Mesh::Grid<dim>* grid, Physics<dim>* physics) : 
        GridBoundaries<dim>(grid,physics) {
        if (dim == 1) {
            std::vector<int> leftIds = grid->leftMost();  
            std::vector<int> leftcol;
            leftcol.push_back(1);

            std::vector<int> rightIds = grid->rightMost();
            std::vector<int> rightcol;
            rightcol.push_back(rightIds[0]-1);

            boundaryIds.push_back(leftIds);
            boundaryIds.push_back(leftcol);
            boundaryIds.push_back(rightIds);
            boundaryIds.push_back(rightcol);
        }
        else if (dim == 2) {
            std::vector<int> leftIds = grid->leftMost();  
            std::vector<int> leftcol;

            std::vector<int> rightIds = grid->rightMost();
            std::vector<int> rightcol;

            std::vector<int> topIds = grid->topMost();
            std::vector<int> topcol;

            std::vector<int> bottomIds = grid->bottomMost();
            std::vector<int> bottomcol;
            
            for (int j=0; j<bottomIds.size(); ++j) {
                leftcol.push_back(grid->index(1,j));
                rightcol.push_back(grid->index(bottomIds.size()-2,j));
            }

            for (int i=0; i<leftIds.size(); ++i) {
                topcol.push_back(grid->index(i,1));
                bottomcol.push_back(grid->index(i,leftIds.size()-2));
            }

            boundaryIds.push_back(leftIds);
            boundaryIds.push_back(leftcol);
            boundaryIds.push_back(rightIds);
            boundaryIds.push_back(rightcol);

            boundaryIds.push_back(topIds);
            boundaryIds.push_back(topcol);
            boundaryIds.push_back(bottomIds);
            boundaryIds.push_back(bottomcol);
        }
    }
    
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
        for (int i=0;i<2*dim;++i) {
            std::vector<int> b = boundaryIds[2*i];
            std::vector<int> c = boundaryIds[2*i+1];
            CopyBoundaryData(field,b,c);
        }
    }

    void ApplyOutflowBoundary(Field<Lin::Vector<dim>>* field, Mesh::Grid<dim>* grid) {
        for (int i=0;i<2*dim;++i) {
            std::vector<int> b = boundaryIds[2*i];
            std::vector<int> c = boundaryIds[2*i+1];
            CopyBoundaryData(field,b,c);
        }
    }

    void CopyBoundaryData(Field<double>* field, const std::vector<int>& boundaryIds, const std::vector<int>& copyIds) {
        for (int i=0;i<boundaryIds.size();++i)
            field->setValue(boundaryIds[i],field->getValue(copyIds[i]));
    }

    void CopyBoundaryData(Field<Lin::Vector<dim>>* field, const std::vector<int>& boundaryIds, const std::vector<int>& copyIds) {
        for (int i=0;i<boundaryIds.size();++i)
            field->setValue(boundaryIds[i],field->getValue(copyIds[i]));
    }
};

