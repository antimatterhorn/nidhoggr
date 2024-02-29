#include "grid.h"
#include <iostream>
#include <random>

const int GRID_SIZE_X = 100;
const int GRID_SIZE_Y = 100;
const double DIFFUSION_RATE = 0.1;
const double REACTION_RATE = 0.05;

Grid::Grid() : x1(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0)),
               x2(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0)),
               x3(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0)) {}

void Grid::initialize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);

    for (int i = 0; i < GRID_SIZE_X; ++i) {
        for (int j = 0; j < GRID_SIZE_Y; ++j) {
            int choice = dis(gen);
            if (choice == 0) {
                x1[i][j] = 1.0; // or any other initial concentration value for x1
            } else if (choice == 1) {
                x2[i][j] = 1.0; // or any other initial concentration value for x2
            } else {
                x3[i][j] = 1.0; // or any other initial concentration value for x3
            }
        }
    }
}

void Grid::update() {
    std::vector<std::vector<double>> new_x1(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0));
    std::vector<std::vector<double>> new_x2(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0));
    std::vector<std::vector<double>> new_x3(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0));

    // Iteration and updating logic here...
}

void Grid::print() {
    for (int i = 0; i < GRID_SIZE_X; ++i) {
        for (int j = 0; j < GRID_SIZE_Y; ++j) {
            std::cout << "(" << x1[i][j] << ", " << x2[i][j] << ", " << x3[i][j] << ") ";
        }
        std::cout << std::endl;
    }
}
