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

        std::vector<std::shared_ptr<FieldBase>> _extraFields;
        std::vector<int> lm,rm,tm,bm,fm,km;
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

        inline int getnx() const    { return nx; };
        inline int getny() const    { return ny; };
        inline int getnz() const    { return nz; };
        inline int size_x() const   { return nx; };
        inline int size_y() const   { return ny; };
        inline int size_z() const   { return nz; };
        inline int size() const     { return nx*ny*nz; };
        inline double getdx() const { return dx; };
        inline double getdy() const { return dy; };
        inline double getdz() const { return dz; };
        inline std::vector<int> leftMost() const    { return lm; };
        inline std::vector<int> rightMost() const   { return rm; };
        inline std::vector<int> topMost() const     { return tm; };
        inline std::vector<int> bottomMost() const  { return bm; };
        inline std::vector<int> frontMost() const   { return fm; };
        inline std::vector<int> backMost() const    { return km; };

        double spacing(int axis) const;
        Lin::Vector<dim> getPosition(int id);

        std::vector<int> getNeighboringCells(int idx) const;
        std::array<int, 3> indexToCoordinates(int idx) const;

        void findBoundaries(const int buffer);
        bool onBoundary(const int idx);
        void assignPositions(NodeList* nodeList);

        template <typename T>
        void insertField(const std::string& name);

        template <typename T>
        Field<T>* getField(const std::string& name);
    };
}


#include "grid.cc"

#endif // GRID_HH
