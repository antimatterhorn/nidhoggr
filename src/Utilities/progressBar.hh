#include <iostream>

#include <cmath> // for round

void ProgressBar(double pct, std::string text) {
    // Define the length of the progress bar
    const int barWidth = 20;

    // Calculate the number of filled positions
    int filledLength = static_cast<int>(round(pct * barWidth));

    // Create the progress bar string
    std::string bar = "[";
    for (int i = 0; i < filledLength; ++i) {
        bar += "=";
    }
    bar += ">";
    for (int i = filledLength + 1; i < barWidth; ++i) {
        bar += "·";
    }
    bar += "]";

    bar += " " + std::to_string(pct*100) + "%";

    bar += " " + text;

    // Print the progress bar
    std::cout << bar << std::endl;
}