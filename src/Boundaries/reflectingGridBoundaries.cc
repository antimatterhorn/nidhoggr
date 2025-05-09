#include <vector>
#include "gridBoundaries.hh"

// Base class for Grid Boundaries
template <int dim>
class ReflectingGridBoundaries : public GridBoundaries<dim> {
protected:
    std::vector<std::vector<int>> boundaryLists;
    std::vector<std::vector<int>> interiorLists;
public:
    using Vector      = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    ReflectingGridBoundaries(Mesh::Grid<dim>* grid) : 
        GridBoundaries<dim>(grid) {
        if constexpr (dim == 1) {
            boundaryLists.push_back(grid->leftMost());
            interiorLists.push_back({1});

            boundaryLists.push_back(grid->rightMost());
            interiorLists.push_back({grid->rightMost()[0]-1});
        }
        else if constexpr (dim == 2) {
            std::vector<int> left = grid->leftMost();
            std::vector<int> right = grid->rightMost();
            std::vector<int> top = grid->topMost();
            std::vector<int> bottom = grid->bottomMost();

            std::vector<int> leftInt, rightInt, topInt, bottomInt;

            for (int j = 0; j < right.size(); ++j) {
                leftInt.push_back(grid->index(1, j));
                rightInt.push_back(grid->index(grid->size_x() - 2, j));
            }

            for (int i = 0; i < bottom.size(); ++i) {
                topInt.push_back(grid->index(i, 1));
                bottomInt.push_back(grid->index(i, grid->size_y() - 2));
            }

            boundaryLists = {left, right, top, bottom};
            interiorLists = {leftInt, rightInt, topInt, bottomInt};
        }
        else if constexpr (dim == 3) {
            std::vector<int> left    = grid->leftMost();
            std::vector<int> right   = grid->rightMost();
            std::vector<int> top     = grid->topMost();
            std::vector<int> bottom  = grid->bottomMost();
            std::vector<int> front   = grid->frontMost();
            std::vector<int> back    = grid->backMost();
        
            std::vector<int> leftInt, rightInt, topInt, bottomInt, frontInt, backInt;
        
            int Nx = grid->size_x();
            int Ny = grid->size_y();
            int Nz = grid->size_z();
        
            // Left and Right
            for (int k = 0; k < Nz; ++k) {
                for (int j = 0; j < Ny; ++j) {
                    leftInt.push_back(grid->index(1, j, k));
                    rightInt.push_back(grid->index(Nx - 2, j, k));
                }
            }
        
            // Top and Bottom
            for (int k = 0; k < Nz; ++k) {
                for (int i = 0; i < Nx; ++i) {
                    topInt.push_back(grid->index(i, 1, k));
                    bottomInt.push_back(grid->index(i, Ny - 2, k));
                }
            }
        
            // Front and Back
            for (int j = 0; j < Ny; ++j) {
                for (int i = 0; i < Nx; ++i) {
                    frontInt.push_back(grid->index(i, j, 1));
                    backInt.push_back(grid->index(i, j, Nz - 2));
                }
            }
        
            boundaryLists = {left, right, top, bottom, front, back};
            interiorLists = {leftInt, rightInt, topInt, bottomInt, frontInt, backInt};
        }        
    }

    virtual ~ReflectingGridBoundaries() = default;

    virtual void ApplyBoundaries(State<dim>& state, NodeList* nodeList) override {
        for (int i = 0; i < state.count(); ++i) {
            FieldBase* field = state.getFieldByIndex(i);
            if (typeid(*field) == typeid(ScalarField)) {
                ApplyReflecting(static_cast<ScalarField*>(field));
            } else if (typeid(*field) == typeid(VectorField)) {
                ApplyReflecting(static_cast<VectorField*>(field));
            }
        }
    }

    void ApplyReflecting(ScalarField* field) {
        for (int i = 0; i < boundaryLists.size(); ++i) {
            const auto& b = boundaryLists[i];
            const auto& iids = interiorLists[i];
            for (size_t j = 0; j < b.size(); ++j) {
                field->setValue(b[j], field->getValue(iids[j]));
            }
        }
    }

    void ApplyReflecting(VectorField* field) {
        for (int i = 0; i < boundaryLists.size(); ++i) {
            const auto& b = boundaryLists[i];
            const auto& iids = interiorLists[i];
            for (size_t j = 0; j < b.size(); ++j) {
                auto val = field->getValue(iids[j]);
                val[i/2] *= -1; // Flip component normal to the boundary
                field->setValue(b[j], val);
            }
        }
    }
};