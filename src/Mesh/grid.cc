#include <vector>
#include <iostream>
#include <any>
#include <map>
#include <string>
#include <cmath>
#include "../Math/vector_math.cc"
#include "../Field/field.cc"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/complex.h>

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

    //std::vector<std::map<std::string, std::any>> stateVectors;
    //std::vector<VectorMath::Vector<dim>> positions;
    FieldList<VectorMath::Vector<dim>> positions;
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

    // Method to add a new variable to the state vector
    // template<typename T>
    // void addStateVariable(const std::string& name, T initialValue) {
    //     for (auto& stateVector : stateVectors) {
    //         stateVector[name] = initialValue;
    //     }
    // }

    void initializeGrid() {
        // Resize the state vectors for each cell
        //stateVectors.resize(nx * ny * nz);
        positions = FieldList<VectorMath::Vector<dim>>(Field<VectorMath::Vector<dim>>("position"),nx*ny*nz);
        int idx = 0;
        // for (auto& stateVector : stateVectors) {
        //     stateVector["id"] = idx++;
        // }

        // Compute and store the position of each cell center
        for (int k = 0; k < nz; ++k) {
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    VectorMath::Vector<dim> position;
                    for (int d = 0; d < dim; ++d) {
                        position.values[d] = (d == 0 ? i * dx + 0.5 * dx : (d == 1 ? j * dy + 0.5 * dy : k * dz + 0.5 * dz));
                    }
                    idx = (k * ny + j) * nx + i;
                    positions[idx] = position;
                }
            }
        }
    }

    int getnx() const { return nx; }
    int getny() const { return ny; }
    int getnz() const { return nz; }
    double getdx() const { return dx; }
    double getdy() const { return dy; }
    double getdz() const { return dz; }

    std::vector<VectorMath::Vector<dim>> getPositions() const { return positions; }
    VectorMath::Vector<dim> getPosition(int id){
        return positions[id];
    }
/*
    // Getter method to access state vector of a cell by ID
    std::map<std::string, std::any>& getStateVector(int id) {
        // it may be possible to simply access the id-th state vector, but maybe with parallelization
        // this wouldn't work in the future. so i'm making this general
        for (auto& stateVector : stateVectors) {
            if (std::any_cast<int>(stateVector["id"]) == id) {
                return stateVector;
            }
        }
        throw std::out_of_range("Cell ID not found");
    }

    // Getter method to access state vector of a cell by ID
    py::dict getState(int id) {
        for (auto& stateVector : stateVectors) {
            auto it = stateVector.find("id");
            if (it != stateVector.end() && std::any_cast<int>(it->second) == id)
                return convertStateVectorToDict(stateVector); // Convert and return state vector
        }

        // If ID not found, throw an exception
        throw std::out_of_range("Cell ID not found");
    }

    // Function to convert the state vector (std::map) to a Python dictionary
    py::dict convertStateVectorToDict(const std::map<std::string, std::any>& stateVector) {
        py::dict py_state_vector;
        for (auto const& entry : stateVector) {
            py::str key = py::str(entry.first);
            py_state_vector[key] = pybind11::cast(entry.second);  // Assuming this still works
        }
        return py_state_vector;
    }

    // Getter method to retrieve position of a cell by ID
    VectorMath::Vector<dim> getPosition(int id) {
        return std::any_cast<VectorMath::Vector<dim>>(getState(id)["position"]);
    }
*/
};
}