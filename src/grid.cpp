#include <iostream>
#include <vector>
#include <random>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class Grid {

private:
    std::vector<std::vector<std::vector<double>>> xx;
    std::vector<std::vector<double>> rho;
    std::vector<std::vector<double>> maxC;
    int gsize_x;
    int gsize_y;
    int nConc;      // number of concentrations
    int nConn;      // number of connections
    double A;       // reaction coefficient
    double D;       // diffusion coefficient

public:
    Grid(int n, int size_x, int size_y) : nConc(n), gsize_x(size_x), gsize_y(size_y),
        xx(n, std::vector<std::vector<double>>(size_x, std::vector<double>(size_y, 0))),
        rho(size_x, std::vector<double>(size_y, 0)),
        maxC(size_x, std::vector<double>(size_y, 0)) {
            //if (nConc %2 == 0)
            if (nConc != 3) {
                //std::cout << "Need odd number of concentrations for this implementation." << std::endl;
                std::cout << "Need 3 concentrations for this implementation." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            nConn = (n-1)<<1;
            A = 0.2;
            D = 0.7;
        }

    void initialize() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, nConc-1);

        for (int i = 0; i < gsize_x; ++i) {
            for (int j = 0; j < gsize_y; ++j) {
                int choice = dis(gen);
                xx[choice][i][j] = 1.0; // or any other initial concentration value for this choice
            }
        }
    }

    void update() {
        // Temporary grid to store updated concentrations
        std::vector<std::vector<std::vector<double>>> updated_xx(nConc, std::vector<std::vector<double>>(gsize_x, std::vector<double>(gsize_y, 0)));        

        for(int i=0;i<gsize_x;++i) {
            for(int j=0;j<gsize_y;++j) {
                rho[i][j] = 0;
                for(int c=0;c<nConc;++c) 
                    rho[i][j] += xx[c][i][j];           
            }
        }
/*
This simulation shows a solution of a reaction-diffusion equation that imitates the game. There are five chemicals, which diffuse, and transform into each other. Each chemical can convert two other ones into its own type, while it is converted by the other two. 
The reaction-diffusion equation is defined as follows. Denoting by rho = u + v + w + x + y  the total concentration, the system of equations is given by 
d_t u = D*Delta(u) + u*(1 - rho - a*v - a*x)
d_t v = D*Delta(v) + v*(1 - rho - a*w - a*y)
d_t w = D*Delta(w) + w*(1 - rho - a*x - a*u)
d_t x = D*Delta(x) + x*(1 - rho - a*y - a*v)
d_t y = D*Delta(y) + y*(1 - rho - a*u - a*w)

d_t u = D*Delta(u) + u*(1 - rho - a*v)
d_t v = D*Delta(v) + v*(1 - rho - a*w)
d_t w = D*Delta(w) + w*(1 - rho - a*u)
where Delta denotes the Laplace operator, which performs a local average, and the parametesr a and D are equal to 0.75 and 0.075. Terms such as a*v, a*x, and so on denote reaction terms, in which one of the five chemicals is beaten by another one. 
*/

        for(int c=0;c<nConc;++c) {
            for(int i=0;i<gsize_x;++i) {
                for(int j=0;j<gsize_y;++j) {
                    int nc = (c+1)%nConc;   // this only works for 3 concentrations!
                    double r = rho[i][j];
                    double u = xx[c][i][j];
                    double dudt = D*deriv(c,i,j);
                    dudt += u*(1.0-r);
                    dudt -= u*A*xx[nc][i][j];
                    updated_xx[c][i][j] = std::max(std::min(1.0,xx[c][i][j]+dudt),0.0);
                    // this assumes dt = 1 which is fine for now
                }
            }
        }


        // Copy updated concentrations back to xx
        xx = updated_xx;
    }

    double deriv(int c, int x, int y) {
        double del = 0;
        double thisCon = xx[c][x][y];
        for(int i=x-1;i<x+2;++i) {
            for(int j=y-1;j<y+2;++j) {
                int ii = ((i%gsize_x)+gsize_x)%gsize_x; // this is like python's mod op
                int jj = ((j%gsize_y)+gsize_y)%gsize_y;
                double fac = 0;
                if(ii!=x && jj!=y)
                    fac = 0.05; 
                else
                    fac = 0.2;
                double thatCon = xx[c][ii][jj];
                //del += (thatCon-thisCon)/fac;
                del += thatCon*fac; // this is a kernel based approach
            }
        }
        return del;
    }

    void print() {
        for (int i = 0; i < gsize_x; ++i) {
            for (int j = 0; j < gsize_y; ++j) {
                std::cout << "(";
                for (int d =0; d < nConc-1; ++d) {
                    std::cout << xx[d][i][j] << ",";
                }
                std::cout << xx[nConc-1][i][j] << ") ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<double> getCell(int i, int j) {
        std::vector<double> cell(nConc);
        for (int c=0; c<nConc; ++c)
            cell[c] = xx[c][i][j];
        return cell;
    }

    std::vector<std::vector<std::vector<double>>> getxx() {
        return xx;
    }

        // Getter for diffusion coefficient
    double getD() const {
        return D;
    }

    // Setter for diffusion coefficient
    void setD(double value) {
        D = value;
    }

    // Getter for reaction coefficient
    double getA() const {
        return A;
    }

    // Setter for reaction coefficient
    void setA(double value) {
        A = value;
    }
};
