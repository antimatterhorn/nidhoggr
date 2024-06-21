#ifndef GRID_CC
#define GRID_CC

#include "grid.hh"

namespace Mesh {
    template <int dim>
    Grid<dim>::Grid(int num_cells_x, double spacing_x)
        : nx(num_cells_x), ny(1), nz(1), dx(spacing_x), dy(0.0), dz(0.0) {
        initializeGrid();
    }

    template <int dim>
    Grid<dim>::Grid(int num_cells_x, int num_cells_y, double spacing_x, double spacing_y)
        : nx(num_cells_x), ny(num_cells_y), nz(1), dx(spacing_x), dy(spacing_y), dz(0.0) {
        initializeGrid();
    }

    template <int dim>
    Grid<dim>::Grid(int num_cells_x, int num_cells_y, int num_cells_z, double spacing_x, double spacing_y, double spacing_z)
        : nx(num_cells_x), ny(num_cells_y), nz(num_cells_z), dx(spacing_x), dy(spacing_y), dz(spacing_z) {
        initializeGrid();
    }

    template <int dim>
    void 
    Grid<dim>::initializeGrid() {
        gridPositions = Field<Lin::Vector<dim>>("gridPosition");

        // Compute and store the position of each cell center
        for (int k = 0; k < nz; ++k) {
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    Lin::Vector<dim> gridPosition;
                    for (int d = 0; d < dim; ++d) {
                        gridPosition.values[d] = (d == 0 ? i * dx + 0.5 * dx : (d == 1 ? j * dy + 0.5 * dy : k * dz + 0.5 * dz));
                    }
                    gridPositions.addValue(gridPosition);   
                }
            }
        }

        findBoundaries(1);
    }

    template <int dim>
    void 
    Grid<dim>::setOrigin(Lin::Vector<dim> origin) {
        for (int i = 0; i < gridPositions.getSize(); ++i) {
            gridPositions[i] -= origin;
        }
    }

    // Specialization for 1D
    template <>
    int Grid<1>::index(int i, int, int) const {
        return i;
    }

    // Specialization for 2D
    template <>
    int Grid<2>::index(int i, int j, int) const {
        return j * nx + i;
    }

    // Specialization for 3D
    template <>
    int Grid<3>::index(int i, int j, int k) const {
        return (k * ny + j) * nx + i;
    }

    // Generic template definition (if you need for other dimensions)
    template <int dim>
    int Grid<dim>::index(int i, int j, int k) const {
        // This is a generic case. You can define custom behavior or throw an error if needed.
        static_assert(dim <= 3, "Only up to 3 dimensions are supported");
        return -1; // Error value
    }

    template <int dim>
    double 
    Grid<dim>::spacing(int axis) const {
        if (axis==0)
            return dx;
        else if (axis==1)
            return dy;
        else if (axis==2)
            return dz;
        return 1;
    }

    template <int dim>
    Lin::Vector<dim> 
    Grid<dim>::getPosition(int id) {
        return gridPositions[id];
    }

    template <int dim>
    std::vector<int> 
    Grid<dim>::getNeighboringCells(int idx) const {
        std::array<int, 3> coords = indexToCoordinates(idx);
        std::vector<int> neighbors;

        // Check right neighbor
        if (coords[0] < nx - 1) {
            int rightIdx = index(coords[0] + 1, coords[1], coords[2]);
            neighbors.push_back(rightIdx);
        }

        // Check left neighbor
        if (coords[0] > 0) {
            int leftIdx = index(coords[0] - 1, coords[1], coords[2]);
            neighbors.push_back(leftIdx);
        }

        // Check down neighbor
        if (coords[1] < ny - 1) {
            int downIdx = index(coords[0], coords[1] + 1, coords[2]);
            neighbors.push_back(downIdx);
        }

        // Check up neighbor
        if (coords[1] > 0) {
            int upIdx = index(coords[0], coords[1] - 1, coords[2]);
            neighbors.push_back(upIdx);
        }

        // Check back neighbor (for 3D grid)
        if constexpr (dim == 3) {
            if (coords[2] < nz - 1) {
                int backIdx = index(coords[0], coords[1], coords[2] + 1);
                neighbors.push_back(backIdx);
            }
        }

        // Check front neighbor (for 3D grid)
        if constexpr (dim == 3) {
            if (coords[2] > 0) {
                int frontIdx = index(coords[0], coords[1], coords[2] - 1);
                neighbors.push_back(frontIdx);
            }
        }

        return neighbors;
    }

    template <int dim>
    std::array<int, 3> 
    Grid<dim>::indexToCoordinates(int idx) const {
        std::array<int, 3> coords;
        coords.fill(0);
        if constexpr (dim == 3) {
            coords[2] = idx / (nx * ny); // Compute k
            idx -= coords[2] * nx * ny;
        }
        if constexpr (dim > 1) 
            coords[1] = idx / nx; // Compute j
        coords[0] = idx % nx; // Compute i
        return coords;
    }

    template <int dim>
    void
    Grid<dim>::findBoundaries(const int buffer) {
        lm.clear();
        rm.clear();
        tm.clear();
        bm.clear();
        fm.clear();
        km.clear();
        
        if constexpr (dim == 1) {
            for (int b=0; b<buffer; ++b) {
                lm.push_back(index(b));
                rm.push_back(index(nx - 1 - b));
            }
        }
        else if constexpr (dim == 2) {
            for (int b=0; b<buffer; ++b) {
                for (int j = 0; j < ny; ++j) {
                    lm.push_back(index(b, j));
                    rm.push_back(index(nx - 1 - b, j));
                }
                for (int i = 0; i < nx; ++i) {
                    tm.push_back(index(i, b));
                    bm.push_back(index(i, ny - 1 - b));
                }
            }
        }
        else if constexpr (dim == 3) {
            for (int b=0; b<buffer; ++b) {
                for (int j = 0; j < ny; ++j) {
                    for (int k = 0; k < nz; ++k) {
                        lm.push_back(index(b, j, k));
                        rm.push_back(index(size() - 1 - b, j, k));
                    }
                }
                for (int i = 0; i < nx; ++i) {
                    for (int k = 0; k < nz; ++k) {
                        tm.push_back(index(i, b, k));
                        bm.push_back(index(i, ny - 1 - b, k));
                    }
                }
                for (int i = 0; i < nx; ++i) {
                    for (int j = 0; j < ny; ++j) {
                        fm.push_back(index(i, j, b));
                        km.push_back(index(i, j, nz - 1 - b));
                    }
                }
            }
        }
    }

    template <int dim>
    bool 
    Grid<dim>::onBoundary(const int idx) {
        bool inside = true;
    
        for (int i=0;i<lm.size();++i) {
            if (lm[i]==idx) {
                inside = false;
                break;
            }
        }
        for (int i=0;i<rm.size();++i) {
            if (rm[i]==idx) {
                inside = false;
                break;
            }
        }

        if (dim>1) {
            for (int i=0;i<tm.size();++i) {
                if (tm[i]==idx) {
                    inside = false;
                    break;
                }
            }
            for (int i=0;i<bm.size();++i) {
                if (bm[i]==idx) {
                    inside = false;
                    break;
                }
            } 
        }

        if (dim>2) {
            for (int i=0;i<fm.size();++i) {
                if (fm[i]==idx) {
                    inside = false;
                    break;
                }
            }
            for (int i=0;i<km.size();++i) {
                if (km[i]==idx) {
                    inside = false;
                    break;
                }
            }
        }

        return !inside;
    }

    template <int dim>
    void
    Grid<dim>::assignPositions(NodeList* nodeList) {
        using Vector = Lin::Vector<dim>;
        using VectorField = Field<Vector>;
        if (nodeList->getField<Vector>("position") == nullptr)
            nodeList->insertField<Vector>("position");
                
        VectorField* position = nodeList->getField<Vector>("position");
        for (int i = 0; i < position->size(); ++i) {
            position->setValue(i, this->getPosition(i));
        }
    }

    template <int dim>
    template <typename T>
    void Grid<dim>::insertField(const std::string& name) {
        auto newField = std::make_shared<Field<T>>(name, this->size());
        _extraFields.push_back(newField);
    }

    template <int dim>
    template <typename T>
    Field<T>* Grid<dim>::getField(const std::string& name) {
        for (const auto& field : _extraFields) {
            auto specificField = std::dynamic_pointer_cast<Field<T>>(field);
            if (specificField && specificField->getName() == name) {
                return specificField.get();
            }
        }
        return nullptr;
    }
}


#endif