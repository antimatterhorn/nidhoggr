#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class PeriodicGridBoundaries : public GridBoundaries<dim> {
protected:
    std::vector<std::vector<int>> boundaryLists;
public:
    using Vector      = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;
    
    PeriodicGridBoundaries(Mesh::Grid<dim>* grid) : 
        GridBoundaries<dim>(grid) {
        if (dim == 1) {
            std::vector<int> leftIds = grid->leftMost();  
            std::vector<int> leftcol;
            leftcol.push_back(1);

            std::vector<int> rightIds = grid->rightMost();
            std::vector<int> rightcol;
            rightcol.push_back(rightIds[0]-1);

            boundaryLists.push_back(leftIds);
            boundaryLists.push_back(rightcol); 

            boundaryLists.push_back(rightIds);
            boundaryLists.push_back(leftcol);
            
        }
        else if (dim == 2) {
            std::vector<int> leftIds   = grid->leftMost();  
            std::vector<int> leftcol;

            std::vector<int> rightIds  = grid->rightMost();
            std::vector<int> rightcol;

            std::vector<int> topIds    = grid->topMost();
            std::vector<int> topcol;

            std::vector<int> bottomIds = grid->bottomMost();
            std::vector<int> bottomcol;
            
            for (int j=0; j<rightIds.size(); ++j) {
                leftcol.push_back(grid->index(1,j));
                rightcol.push_back(grid->index(bottomIds.size()-2,j));
            }

            for (int i=0; i<bottomIds.size(); ++i) {
                topcol.push_back(grid->index(i,1));
                bottomcol.push_back(grid->index(i,leftIds.size()-2));
            }

            boundaryLists.push_back(leftIds);
            boundaryLists.push_back(rightcol);

            boundaryLists.push_back(rightIds);
            boundaryLists.push_back(leftcol);

            boundaryLists.push_back(topIds);
            boundaryLists.push_back(bottomcol); 

            boundaryLists.push_back(bottomIds);
            boundaryLists.push_back(topcol);
            
        }
        else if (dim == 3) {
            std::vector<int> leftIds   = grid->leftMost();  
            std::vector<int> leftcol;

            std::vector<int> rightIds  = grid->rightMost();
            std::vector<int> rightcol;

            std::vector<int> topIds    = grid->topMost();
            std::vector<int> topcol;

            std::vector<int> bottomIds = grid->bottomMost();
            std::vector<int> bottomcol;

            std::vector<int> frontIds  = grid->frontMost();
            std::vector<int> frontcol;

            std::vector<int> backIds   = grid->backMost();
            std::vector<int> backcol;

            int Nx = grid->size_x();
            int Ny = grid->size_y();

            for (int k = 0; k < Ny; ++k) {
                for (int j = 0; j < Nx; ++j) {
                    leftcol.push_back(grid->index(1, j, k));
                    rightcol.push_back(grid->index(Nx - 2, j, k));
                }
            }

            for (int k = 0; k < Ny; ++k) {
                for (int i = 0; i < Nx; ++i) {
                    topcol.push_back(grid->index(i, 1, k));
                    bottomcol.push_back(grid->index(i, Ny - 2, k));
                }
            }

            for (int j = 0; j < Nx; ++j) {
                for (int i = 0; i < Ny; ++i) {
                    frontcol.push_back(grid->index(i, j, 1));
                    backcol.push_back(grid->index(i, j, Ny - 2));
                }
            }

            boundaryLists.push_back(leftIds);
            boundaryLists.push_back(rightcol);
            
            boundaryLists.push_back(rightIds);
            boundaryLists.push_back(leftcol);           

            boundaryLists.push_back(topIds);
            boundaryLists.push_back(bottomcol);
          
            boundaryLists.push_back(bottomIds);
            boundaryLists.push_back(topcol);

            boundaryLists.push_back(frontIds);
            boundaryLists.push_back(backcol);
            
            boundaryLists.push_back(backIds);
            boundaryLists.push_back(frontcol);
        }
    }
    
    virtual ~PeriodicGridBoundaries() = default;
    
    virtual void
    ApplyBoundaries(State<dim>& state, NodeList* nodeList) override {
        Mesh::Grid<dim>* grid = this->grid;

        for (int i = 0; i < state.count(); ++i) {
            FieldBase* field = state.getFieldByIndex(i); // Get the field at index i
            if (typeid(*field) == typeid(ScalarField)) {
                ScalarField* doubleField = dynamic_cast<ScalarField*>(field);
                if (doubleField) {
                    ApplyPeriodicBoundary(doubleField);
                }
            } else if (typeid(*field) == typeid(VectorField)) {
                VectorField* vectorField = dynamic_cast<VectorField*>(field);
                if (vectorField) {
                    ApplyPeriodicBoundary(vectorField);
                }
            }
        }
    }

    void ApplyPeriodicBoundary(ScalarField* field) {       
        for (int i=0; i<2*dim;++i) {
            std::vector<int> b = boundaryLists[2*i];
            std::vector<int> c = boundaryLists[2*i+1];
            CopyBoundaryData(field, b, c);
        }
    }

    void ApplyPeriodicBoundary(VectorField* field) {
        for (int i=0; i<2*dim;++i) {
            std::vector<int> b = boundaryLists[2*i];
            std::vector<int> c = boundaryLists[2*i+1];
            CopyBoundaryData(field, b, c);
        }
    }

    void CopyBoundaryData(ScalarField* field, const std::vector<int>& boundaryIds, const std::vector<int>& copyIds) {     
        for (int i=0;i<boundaryIds.size();++i) {
            int bi = boundaryIds[i];
            int ci = copyIds[i];
            field->setValue(bi,field->getValue(ci));
        }      
    }

    void CopyBoundaryData(VectorField* field, const std::vector<int>& boundaryIds, const std::vector<int>& copyIds) {
        for (int i=0;i<boundaryIds.size();++i) {
            int bi = boundaryIds[i];
            int ci = copyIds[i];
            field->setValue(bi,field->getValue(ci));
        }
    }

    std::vector<std::vector<int>> GetBounds(Mesh::Grid<dim>* grid) {
        std::vector<std::vector<int>> retVector;
        retVector.push_back(grid->leftMost());
        retVector.push_back(grid->rightMost());
        if (dim>1) {
            retVector.push_back(grid->topMost());
            retVector.push_back(grid->bottomMost());
        }
        if (dim>2) {
            retVector.push_back(grid->frontMost());
            retVector.push_back(grid->backMost());
        }
        return retVector;
    }
};
