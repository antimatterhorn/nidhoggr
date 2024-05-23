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
    void Grid<dim>::initializeGrid() {
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
    }

    template <int dim>
    void Grid<dim>::setOrigin(Lin::Vector<dim> origin) {
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
    int 
    Grid<dim>::getnx() const {
        return nx;
    }

    template <int dim>
    int 
    Grid<dim>::getny() const {
        return ny;
    }

    template <int dim>
    int 
    Grid<dim>::getnz() const {
        return nz;
    }

    template <int dim>
    int 
    Grid<dim>::size_x() const {
        return nx;
    }

    template <int dim>
    int 
    Grid<dim>::size_y() const {
        return ny;
    }

    template <int dim>
    int 
    Grid<dim>::size_z() const {
        return nz;
    }

    template <int dim>
    int 
    Grid<dim>::size() const {
        return gridPositions.size();
    }

    template <int dim>
    double 
    Grid<dim>::getdx() const {
        return dx;
    }

    template <int dim>
    double 
    Grid<dim>::getdy() const {
        return dy;
    }

    template <int dim>
    double 
    Grid<dim>::getdz() const {
        return dz;
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

        // Check up neighbor
        if (coords[1] > 0) {
            int upIdx = index(coords[0], coords[1] - 1, coords[2]);
            neighbors.push_back(upIdx);
        }

        // Check down neighbor
        if (coords[1] < ny - 1) {
            int downIdx = index(coords[0], coords[1] + 1, coords[2]);
            neighbors.push_back(downIdx);
        }

        // Check front neighbor (for 3D grid)
        if constexpr (dim == 3) {
            if (coords[2] > 0) {
                int frontIdx = index(coords[0], coords[1], coords[2] - 1);
                neighbors.push_back(frontIdx);
            }
        }

        // Check back neighbor (for 3D grid)
        if constexpr (dim == 3) {
            if (coords[2] < nz - 1) {
                int backIdx = index(coords[0], coords[1], coords[2] + 1);
                neighbors.push_back(backIdx);
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
    std::vector<int> 
    Grid<dim>::leftMost() {
        std::vector<int> boundaryIndices;
        if constexpr (dim == 1)
            boundaryIndices.push_back(index(0));
        else if constexpr (dim == 2) {
            for (int j = 0; j < ny; ++j)
                boundaryIndices.push_back(index(0, j));
        }
        else if constexpr (dim == 3) {
            for (int j = 0; j < ny; ++j)
                for (int k = 0; k < nz; ++k)
                    boundaryIndices.push_back(index(0, j, k));
        }
        return boundaryIndices;
    }

    template <int dim>
    std::vector<int> 
    Grid<dim>::rightMost() {
        std::vector<int> boundaryIndices;
        if constexpr (dim == 1)
            boundaryIndices.push_back(index(size() - 1));
        else if constexpr (dim == 2) {
            for (int j = 0; j < ny; ++j)
                boundaryIndices.push_back(index(nx - 1, j));
        }
        else if constexpr (dim == 3) {
            for (int j = 0; j < ny; ++j)
                for (int k = 0; k < nz; ++k)
                    boundaryIndices.push_back(index(nx - 1, j, k));
        }
        return boundaryIndices;
    }

    template <int dim>
    std::vector<int> 
    Grid<dim>::topMost() {
        std::vector<int> boundaryIndices;
        if constexpr (dim == 2) {
            for (int i = 0; i < nx; ++i)
                boundaryIndices.push_back(index(i, 0));
        }
        else if constexpr (dim == 3) {
            for (int i = 0; i < nx; ++i)
                for (int k = 0; k < nz; ++k)
                    boundaryIndices.push_back(index(i, 0, k));
        }
        return boundaryIndices;
    }

    template <int dim>
    std::vector<int> 
    Grid<dim>::bottomMost() {
        std::vector<int> boundaryIndices;
        if constexpr (dim == 2) {
            for (int i = 0; i < nx; ++i)
                boundaryIndices.push_back(index(i, ny - 1));
        }
        else if constexpr (dim == 3) {
            for (int i = 0; i < nx; ++i)
                for (int k = 0; k < nz; ++k)
                    boundaryIndices.push_back(index(i, ny - 1, k));
        }
        return boundaryIndices;
    }

    template <int dim>
    std::vector<int> 
    Grid<dim>::frontMost() {
        std::vector<int> boundaryIndices;
        if constexpr (dim == 3) {
            for (int i = 0; i < nx; ++i)
                for (int j = 0; j < ny; ++j)
                    boundaryIndices.push_back(index(i, j, 0));
        }
        return boundaryIndices;
    }

    template <int dim>
    std::vector<int> 
    Grid<dim>::backMost() {
        std::vector<int> boundaryIndices;
        if constexpr (dim == 3) {
            for (int i = 0; i < nx; ++i)
                for (int j = 0; j < ny; ++j)
                    boundaryIndices.push_back(index(i, j, nz - 1));
        }
        return boundaryIndices;
    }

    template <int dim>
    bool 
    Grid<dim>::onBoundary(const int idx) {
        bool inside = true;
        
        std::vector<int> lm = leftMost();
        std::vector<int> rm = rightMost();
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
            std::vector<int> tm = topMost();
            std::vector<int> bm = bottomMost();
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
            std::vector<int> fm = frontMost();
            std::vector<int> rm = backMost();
            for (int i=0;i<fm.size();++i) {
                if (fm[i]==idx) {
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
        }

        return !inside;
    }
}


#endif