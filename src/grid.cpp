#include <iostream>
#include <vector>
#include <random>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class Grid {

private:
    std::vector<std::vector<std::vector<double>>> xx;
    int gsize_x;
    int gsize_y;
    int nConc;

public:
    Grid(int n, int size_x, int size_y) : nConc(n), gsize_x(size_x), gsize_y(size_y),
        xx(n, std::vector<std::vector<double>>(size_x, std::vector<double>(size_y, 0))) {}

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

    void update() {}

    void print() {
        for (int i = 0; i < gsize_x; ++i) {
            for (int j = 0; j < gsize_y; ++j) {
                std::cout << "(";
                for (int d =0; d < nConc; ++d)
                {
                    std::cout << xx[d][i][j] << ",";
                }
                std::cout << ") ";
            }
            std::cout << std::endl;
        }
    }
};
