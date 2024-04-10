#include <vector>

template <int dim>
class Grid {
private:
    int nx; // Number of grid cells in x-direction
    int ny; // Number of grid cells in y-direction
    int nz; // Number of grid cells in z-direction
    double dx; // Grid spacing in x-direction
    double dy; // Grid spacing in y-direction
    double dz; // Grid spacing in z-direction

    std::vector<double> coords;

public:
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

    void initializeGrid() {
        // Resize the grid coordinate array
        coords.resize(nx * ny * nz * dim);

        // Loop through grid cells and assign coordinates
        for (int k = 0; k < nz; ++k) {
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    // Compute coordinates of the grid cell center
                    for (int d = 0; d < dim; ++d) {
                        coords[((k * ny + j) * nx + i) * dim + d] = (d == 0 ? i * dx + 0.5 * dx : (d == 1 ? j * dy + 0.5 * dy : k * dz + 0.5 * dz));
                    }
                }
            }
        }
    }

    void printGridCoordinates() {
        int idx = 0;
        for (int k = 0; k < nz; ++k) {
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    std::cout << "(";
                    for (int d = 0; d < dim; ++d) {
                        std::cout << coords[idx++] << (d < dim - 1 ? ", " : "");
                    }
                    std::cout << ") ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

    int getnx() const { return nx; }
    int getny() const { return ny; }
    int getnz() const { return nz; }
    double getdx() const { return dx; }
    double getdy() const { return dy; }
    double getdz() const { return dz; }
};
