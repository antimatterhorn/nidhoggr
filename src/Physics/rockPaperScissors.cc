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
    Field<std::array<double, 3>> xx;
    Field<double> rho;

    double A;
    double D;
    unsigned int n;

    double 
    deriv(int c, int x, int y) {
        double del = 0;
        int idx = grid.index(x,y);
        double thisCon = xx[idx][c];
        for(int j=y-1;j<y+2;++j) {
            for(int i=x-1;i<x+2;++i) {
                int ii = ((i%grid.size_x())+grid.size_x())%grid.size_x(); // this is like python's mod op
                int jj = ((j%grid.size_y())+grid.size_y())%grid.size_y();
                double fac = 0;
                if(ii!=x && jj!=y)
                    fac = 0.05; 
                else
                    fac = 0.2;
                int iidx = grid.index(ii,jj);
                double thatCon = xx[iidx][c];
                del += thatCon*fac; // this is a kernel based approach
            }
        }
        return del;
    }
public:
    RockPaperScissors(Mesh::Grid<2> grid, double A, double D) : 
        grid(grid),
        A(A),
        D(D),
        n(grid.size()) {
            xx = Field<std::array<double, 3>>("xx",n);
            rho = Field<double>("rho",n);
            this->initialize();
    }

    void
    initialize() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);
        for (int j = 0; j < grid.size_y(); ++j) {
            for (int i = 0; i < grid.size_x(); ++i) {
                int choice = dis(gen);
                int idx = grid.index(i,j);
                xx[idx][choice] = 1.0; // or any other initial concentration value for this choice
            }
        }
    }

    void
    update() {
        // Temporary grid to store updated concentrations
        Field<std::array<double, 3>> updated_xx("temp",xx.size()); 

        for (int j = 0; j < grid.size_y(); ++j) {
            for (int i = 0; i < grid.size_x(); ++i) {
                int idx = grid.index(i,j);
                rho[idx] = 0;
                for(int c=0;c<3;++c) 
                    rho[idx] += xx[idx][c];           
            }
        }
        for(int c=0;c<3;++c) {
            for (int j = 0; j < grid.size_y(); ++j) {
                for (int i = 0; i < grid.size_x(); ++i) {
                    int idx = grid.index(i,j);
                    int nc = (c+1)%3;   // this only works for 3 concentrations!
                    double r = rho[idx];
                    double u = xx[idx][c];
                    double dudt = D*deriv(c,i,j);
                    dudt += u*(1.0-r);
                    dudt -= u*A*xx[idx][nc];
                    updated_xx[idx][c] = std::max(std::min(1.0,xx[idx][c]+dudt),0.0);
                    // this assumes dt = 1 which is fine for now
                }
            }
        }

        xx = updated_xx;
    }

    void
    step(unsigned int nsteps) {
        for(unsigned int i=0;i<nsteps;++i)
        {
            update();
        }
    }

    std::array<double, 3>
    getCell(int i, int j) {
        int idx = grid.index(i,j);
        return xx[idx];
    }
};
}