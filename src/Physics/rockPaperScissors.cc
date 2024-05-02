/*
This simulates a kind of reaction diffusion model with rock-paper-scissors rules.
Not general enough to be used for anything physical. 
*/

#include "../DataBase/field.hh"
#include "../DataBase/nodeList.hh"
#include "../Math/vectorMath.hh"
#include "../Mesh/grid.hh"
#include <random>

namespace Physics {
class RockPaperScissors {
private:
    Mesh::Grid<2> grid;
    //unsigned int nConc;
    Field<Lin::Vector<3>> xx;
    Field<double> rho;
    Field<double> maxC;

    double A;
    double D;
public:
    RockPaperScissors(Mesh::Grid<2> grid, double A, double D) : 
        grid(grid),
        A(A),
        D(D) {
            unsigned int n = grid.size();
            xx.fill(n,Lin::Vector<3>());
            rho.fill(n,0.0);
            maxC.fill(n,0.0);
    }

    void
    initialize() {
        std::random_device rd;
        std::mt19937 gen(rd());
        //std::uniform_int_distribution<> dis(0, nConc-1);
        std::uniform_int_distribution<> dis(0, 2);
        for (int j = 0; j < grid.size_y(); ++j) {
            for (int i = 0; i < grid.size_x(); ++i) {
                int choice = dis(gen);
                int idx = j * grid.size_x() + i;
                xx[idx][choice] = 1.0; // or any other initial concentration value for this choice
            }
        }
    }
};
}