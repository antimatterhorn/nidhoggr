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
    double dx; // Grid spacing in x-direction
    double dy; // Grid spacing in y-direction
    double dz; // Grid spacing in z-direction

    
public:
    Field<Lin::Vector<dim>> positions;
    
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
        positions = Field<Lin::Vector<dim>>("position");

        // Compute and store the position of each cell center
        for (int k = 0; k < nz; ++k) {
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    Lin::Vector<dim> position;
                    for (int d = 0; d < dim; ++d) {
                        position.values[d] = (d == 0 ? i * dx + 0.5 * dx : (d == 1 ? j * dy + 0.5 * dy : k * dz + 0.5 * dz));
                    }
                    positions.addValue(position);   
                }
            }
        }
    }

    void 
    setOrigin(Lin::Vector<dim> origin) {
        for (int i = 0; i < positions.getSize(); ++i) {
            positions[i] -= origin; // this is very not correct
        }
    }

    int
    index(int i) { return i; }

    int
    index(int i, int j) { return j * nx + i; }

    int
    index(int i, int j, int k) { return (k * ny + j) * nx + i; }

    int getnx() const { return nx; }
    int getny() const { return ny; }
    int getnz() const { return nz; }
    int size_x() const { return nx; }
    int size_y() const { return ny; }
    int size_z() const { return nz; }
    int size() const { return positions.size(); }
    double getdx() const { return dx; }
    double getdy() const { return dy; }
    double getdz() const { return dz; }

    Lin::Vector<dim> 
    getPosition(int id){
        return positions[id];
    }
};
}

#endif // GRID_HH