#include <iostream>
#include <vector>
#include <random>

// Define grid parameters
const int GRID_SIZE_X = 100;
const int GRID_SIZE_Y = 100;
const double DIFFUSION_RATE = 10.0;
const double REACTION_RATE = 0.21;

// Define the grid class
class Grid {
private:
    std::vector<std::vector<double>> x1;
    std::vector<std::vector<double>> x2;
    std::vector<std::vector<double>> x3;

public:
    Grid() : x1(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0)),
             x2(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0)),
             x3(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0)) {}

    // Initialize the grid with initial concentrations
    void initialize() {
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

    // Update the grid according to the reaction-diffusion equation
    void update() {
        std::vector<std::vector<double>> new_x1(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0));
        std::vector<std::vector<double>> new_x2(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0));
        std::vector<std::vector<double>> new_x3(GRID_SIZE_X, std::vector<double>(GRID_SIZE_Y, 0));

        // Iterate through each cell
        for (int i = 0; i < GRID_SIZE_X; ++i) {
            for (int j = 0; j < GRID_SIZE_Y; ++j) {
                // Calculate diffusion
                double diffusion_x1 = DIFFUSION_RATE * (x1[(i+1)%GRID_SIZE_X][j] + x1[(i-1+GRID_SIZE_X)%GRID_SIZE_X][j] +
                                                         x1[i][(j+1)%GRID_SIZE_Y] + x1[i][(j-1+GRID_SIZE_Y)%GRID_SIZE_Y] - 4 * x1[i][j]);
                double diffusion_x2 = DIFFUSION_RATE * (x2[(i+1)%GRID_SIZE_X][j] + x2[(i-1+GRID_SIZE_X)%GRID_SIZE_X][j] +
                                                         x2[i][(j+1)%GRID_SIZE_Y] + x2[i][(j-1+GRID_SIZE_Y)%GRID_SIZE_Y] - 4 * x2[i][j]);
                double diffusion_x3 = DIFFUSION_RATE * (x3[(i+1)%GRID_SIZE_X][j] + x3[(i-1+GRID_SIZE_X)%GRID_SIZE_X][j] +
                                                         x3[i][(j+1)%GRID_SIZE_Y] + x3[i][(j-1+GRID_SIZE_Y)%GRID_SIZE_Y] - 4 * x3[i][j]);

                // Calculate reaction
                double reaction_x1 = REACTION_RATE * (x1[i][j] * x2[i][j] - x3[i][j]);
                double reaction_x2 = REACTION_RATE * (x1[i][j] * x2[i][j] - x3[i][j]);
                double reaction_x3 = REACTION_RATE * (x3[i][j] - x1[i][j] * x2[i][j]);

                // Update concentrations
                new_x1[i][j] = x1[i][j] + diffusion_x1 + reaction_x1;
                new_x2[i][j] = x2[i][j] + diffusion_x2 + reaction_x2;
                new_x3[i][j] = x3[i][j] + diffusion_x3 + reaction_x3;
            }
        }

        // Update grid with new concentrations
        x1 = new_x1;
        x2 = new_x2;
        x3 = new_x3;
    }

    // Print the grid
    void print() {
        for (int i = 0; i < GRID_SIZE_X; ++i) {
            for (int j = 0; j < GRID_SIZE_Y; ++j) {
                std::cout << "(" << x1[i][j] << ", " << x2[i][j] << ", " << x3[i][j] << ") ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    Grid grid;
    grid.initialize();

    // Run simulation for a certain number of steps
    for (int step = 0; step < 100; ++step) {
        grid.update();
    }

    // Print final state
    grid.print();

    return 0;
}
