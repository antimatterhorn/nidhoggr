#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid {
private:
    std::vector<std::vector<double>> x1;
    std::vector<std::vector<double>> x2;
    std::vector<std::vector<double>> x3;

public:
    Grid();
    void initialize();
    void update();
    void print();
};

#endif // GRID_H
