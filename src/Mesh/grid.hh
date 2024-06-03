#ifndef GRID_HH
#define GRID_HH

#include <vector>
#include <string>
#include "../Math/vectorMath.hh"
#include "../DataBase/field.hh"
#include "../DataBase/nodeList.hh"

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

        Grid(int num_cells_x, double spacing_x);
        Grid(int num_cells_x, int num_cells_y, double spacing_x, double spacing_y);
        Grid(int num_cells_x, int num_cells_y, int num_cells_z, double spacing_x, double spacing_y, double spacing_z);

        void initializeGrid();
        void setOrigin(Lin::Vector<dim> origin);

        int index(int i, int j = 0, int k = 0) const;

        int getnx() const;
        int getny() const;
        int getnz() const;
        int size_x() const;
        int size_y() const;
        int size_z() const;
        int size() const;
        double getdx() const;
        double getdy() const;
        double getdz() const;
        double spacing(int axis) const;
        Lin::Vector<dim> getPosition(int id);

        std::vector<int> getNeighboringCells(int idx) const;
        std::array<int, 3> indexToCoordinates(int idx) const;
        std::vector<int> leftMost();
        std::vector<int> rightMost();
        std::vector<int> topMost();
        std::vector<int> bottomMost();
        std::vector<int> frontMost();
        std::vector<int> backMost();
        bool onBoundary(const int idx);

        void assignPositions(NodeList* nodeList);
    };
}


#include "grid.cc"

#endif // GRID_HH
