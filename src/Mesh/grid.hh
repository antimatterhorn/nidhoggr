#ifndef GRID_HH
#define GRID_HH

#include <vector>
#include <string>
#include "../Math/vectorMath.hh"
#include "../DataBase/field.hh"

namespace Mesh {
template <int dim>
class Grid {
private:
    int nx; // Number of grid cells in x-direction
    int ny; // Number of grid cells in y-direction
    int nz; // Number of grid cells in z-direction
 
public:
    double dx; // Grid spacing in x-direction
    double dy; // Grid spacing in y-direction
    double dz; // Grid spacing in z-direction

    Field<Lin::Vector<dim>> gridPositions;
    
    // Constructor for 1D grid
    Grid(int num_cells_x, double spacing_x) 
        : nx(num_cells_x), ny(1), nz(1), dx(spacing_x), dy(0.0), dz(0.0) {
        // Initialize the grid coordinates
        initializeGrid();
    }

    // Constructor for 2D grid
    Grid(int num_cells_x, int num_cells_y, double spacing_x, double spacing_y) 
        : nx(num_cells_x), ny(num_cells_y), nz(1), dx(spacing_x), dy(spacing_y), dz(0.0) {
        // Initialize the grid coordinates
        initializeGrid();
    }

    // Constructor for 3D grid
    Grid(int num_cells_x, int num_cells_y, int num_cells_z, double spacing_x, double spacing_y, double spacing_z) 
        : nx(num_cells_x), ny(num_cells_y), nz(num_cells_z), dx(spacing_x), dy(spacing_y), dz(spacing_z) {        
        // Initialize the grid coordinates
        initializeGrid();
    }

    void 
    initializeGrid() {
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

    void 
    setOrigin(Lin::Vector<dim> origin) {
        for (int i = 0; i < gridPositions.getSize(); ++i) {
            gridPositions[i] -= origin; // this is very not correct
        }
    }

    int
    index(int i) const { return i; }

    int
    index(int i, int j) const { return j * nx + i; }

    int
    index(int i, int j, int k) const { return (k * ny + j) * nx + i; }

    int getnx() const { return nx; }
    int getny() const { return ny; }
    int getnz() const { return nz; }
    int size_x() const { return nx; }
    int size_y() const { return ny; }
    int size_z() const { return nz; }
    int size() const { return gridPositions.size(); }
    double getdx() const { return dx; }
    double getdy() const { return dy; }
    double getdz() const { return dz; }

    Lin::Vector<dim> 
    getPosition(int id){ return gridPositions[id]; }

    std::vector<int> getNeighboringCells(int idx) const {
        std::array<int, 3> coords = indexToCoordinates(idx);
        std::vector<int> neighbors;

        // Check left neighbor
        if (coords[0] > 0) {
            int leftIdx = index(coords[0] - 1, coords[1], coords[2]);
            neighbors.push_back(leftIdx);
        }

        // Check right neighbor
        if (coords[0] < nx - 1) {
            int rightIdx = index(coords[0] + 1, coords[1], coords[2]);
            neighbors.push_back(rightIdx);
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

    std::array<int, 3> indexToCoordinates(int idx) const {
        std::array<int, 3> coords;
        coords.fill(0);
        if constexpr (dim == 3) {
            coords[2] = idx / (nx * ny); // Compute k
            idx -= coords[2] * nx * ny;
        }
        coords[1] = idx / nx; // Compute j
        coords[0] = idx % nx; // Compute i
        return coords;
    }

    std::vector<int>
    leftMost() {
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

    std::vector<int>
    rightMost() {
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

    std::vector<int>
    topMost() {
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

    std::vector<int>
    bottomMost() {
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

    std::vector<int>
    frontMost() {
        std::vector<int> boundaryIndices;
        if constexpr (dim == 3) {
            for (int i = 0; i < nx; ++i)
                for (int j = 0; j < ny; ++j)
                    boundaryIndices.push_back(index(i, j, 0));
        }
        return boundaryIndices;
    }

    std::vector<int>
    backMost() {  // hindmost?
        std::vector<int> boundaryIndices;
        if constexpr (dim == 3) {
            for (int i = 0; i < nx; ++i)
                for (int j = 0; j < ny; ++j)
                    boundaryIndices.push_back(index(i, j, nz - 1));
        }
        return boundaryIndices;
    }

};
}

#endif // GRID_HH