#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class PacmanGridBoundaries : public GridBoundaries<dim> {
protected:

public:
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
            if (typeid(*field) == typeid(Field<double>)) {
                Field<double>* doubleField = dynamic_cast<Field<double>*>(field);
                if (doubleField) {
                    std::vector<int> leftIds = grid->leftMost();
                    std::vector<int> rightIds = grid->rightMost();
                    Field<double> left = Field<double>("left",leftIds.size());
                    Field<double> right = Field<double>("right",rightIds.size());
                    SwapFieldData(doubleField,left,right,leftIds,rightIds);
                    
                    if constexpr (dim == 2) {
                        std::vector<int> topIds = grid->topMost();
                        std::vector<int> bottomIds = grid->bottomMost();
                        Field<double> top = Field<double>("top",topIds.size());
                        Field<double> bottom = Field<double>("bottom",bottomIds.size());
                        SwapFieldData(doubleField,top,bottom,topIds,bottomIds);
                    }
                    else if constexpr (dim == 3) {
                        std::vector<int> frontIds = grid->frontMost();
                        std::vector<int> backIds = grid->backMost();
                        Field<double> front = Field<double>("front",frontIds.size());
                        Field<double> back = Field<double>("back",backIds.size());
                        SwapFieldData(doubleField,front,back,frontIds,backIds);
                    }
                }
            } else if (typeid(*field) == typeid(Field<Lin::Vector<dim>>)) {
                Field<Lin::Vector<dim>>* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                if (vectorField) {
                    std::vector<int> leftIds = grid->leftMost();
                    std::vector<int> rightIds = grid->rightMost();
                    Field<Lin::Vector<dim>> left = Field<Lin::Vector<dim>>("left",leftIds.size());
                    Field<Lin::Vector<dim>> right = Field<Lin::Vector<dim>>("right",rightIds.size());
                    SwapFieldData(vectorField,left,right,leftIds,rightIds);
                    
                    if constexpr (dim == 2) {
                        std::vector<int> topIds = grid->topMost();
                        std::vector<int> bottomIds = grid->bottomMost();
                        Field<Lin::Vector<dim>> top = Field<Lin::Vector<dim>>("top",topIds.size());
                        Field<Lin::Vector<dim>> bottom = Field<Lin::Vector<dim>>("bottom",bottomIds.size());
                        SwapFieldData(vectorField,top,bottom,topIds,bottomIds);
                    }
                    else if constexpr (dim == 3) {
                        std::vector<int> frontIds = grid->frontMost();
                        std::vector<int> backIds = grid->backMost();
                        Field<Lin::Vector<dim>> front = Field<Lin::Vector<dim>>("front",frontIds.size());
                        Field<Lin::Vector<dim>> back = Field<Lin::Vector<dim>>("back",backIds.size());
                        SwapFieldData(vectorField,front,back,frontIds,backIds);
                    }
                }
            }
        }
    }

    virtual void
    SwapFieldData(Field<double>* field, 
                    Field<double>& tempField1, 
                    Field<double>& tempField2, 
                    std::vector<int>& ids1,
                    std::vector<int>& ids2) {
        for (int i = 0; i<ids1.size(); ++i) {
            int id = ids1[i];
            tempField1.setValue(i,field->getValue(id));
        }
        for (int i = 0; i<ids2.size(); ++i) {
            int id = ids2[i];
            tempField2.setValue(i,field->getValue(id));
        }
        for (int i = 0; i<ids1.size(); ++i) {
            int id = ids1[i];
            field->setValue(id,tempField2.getValue(i));
        }
        for (int i = 0; i<ids2.size(); ++i) {
            int id = ids2[i];
            field->setValue(id,tempField1.getValue(i));
        }
    }

    virtual void
    SwapFieldData(Field<Lin::Vector<dim>>* field, 
                    Field<Lin::Vector<dim>>& tempField1, 
                    Field<Lin::Vector<dim>>& tempField2, 
                    std::vector<int>& ids1,
                    std::vector<int>& ids2) {
        for (int i = 0; i<ids1.size(); ++i) {
            int id = ids1[i];
            tempField1.setValue(i,field->getValue(id));
        }
        for (int i = 0; i<ids2.size(); ++i) {
            int id = ids2[i];
            tempField2.setValue(i,field->getValue(id));
        }
        for (int i = 0; i<ids1.size(); ++i) {
            int id = ids1[i];
            field->setValue(id,tempField2.getValue(i));
        }
        for (int i = 0; i<ids2.size(); ++i) {
            int id = ids2[i];
            field->setValue(id,tempField1.getValue(i));
        }
    }
};
