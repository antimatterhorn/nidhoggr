#ifndef IMPORTDEPTHMAP_CC
#define IMPORTDEPTHMAP_CC

#include "importDepthMap.hh"

ImportDepthMap::ImportDepthMap(const std::string& filename) {
    depthMap = loadDepthMap(filename);
}

std::vector<std::vector<double>> 
ImportDepthMap::loadDepthMap(const std::string& filename) {
    std::vector<std::vector<double>> map;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return map;
    }

    std::string line;
    int ncols, nrows;
    double xllcorner, yllcorner, cellsize, NODATA_value;

    // Read header information
    file >> line >> ncols;
    file >> line >> nrows;
    file >> line >> xllcorner;
    file >> line >> yllcorner;
    file >> line >> cellsize;
    file >> line >> NODATA_value;

    printf("%d %d %f %f %f %f\n",ncols,nrows,xllcorner,yllcorner,cellsize,NODATA_value);

    // Read the grid data
    map.resize(nrows, std::vector<double>(ncols));
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            file >> map[i][j];
        }
    }

    file.close();
    return map;
}

double 
ImportDepthMap::bilinearInterpolation(double x, double y,
                                      double q11, double q12, double q21, double q22,
                                      double x1, double x2, double y1, double y2) {
    double r1 = ((x2 - x) / (x2 - x1)) * q11 + ((x - x1) / (x2 - x1)) * q21;
    double r2 = ((x2 - x) / (x2 - x1)) * q12 + ((x - x1) / (x2 - x1)) * q22;
    return ((y2 - y) / (y2 - y1)) * r1 + ((y - y1) / (y2 - y1)) * r2;
}

void 
ImportDepthMap::populateDepthField(Mesh::Grid<2>* grid) {
    using VectorField = Field<double>;

    int mapWidth = depthMap[0].size();
    int mapHeight = depthMap.size();

    
    int gridWidth = grid->size_x();
    int gridHeight = grid->size_y();

    double mapSpacingX = 1.0; // Assume unit spacing for simplicity
    double mapSpacingY = 1.0;

    Field<double>* depthField = grid->template getField<double>("depth");

    // Populate the depth field with interpolated values
    for (int j = 0; j < gridHeight; ++j) {
        for (int i = 0; i < gridWidth; ++i) {
            // Calculate corresponding position in the depth map
            double x = (i + 0.5) * mapWidth / gridWidth;
            double y = (j + 0.5) * mapHeight / gridHeight;

            int x1 = std::max(0, std::min(static_cast<int>(std::floor(x)), mapWidth - 1));
            int x2 = std::max(0, std::min(static_cast<int>(std::ceil(x)), mapWidth - 1));
            int y1 = std::max(0, std::min(static_cast<int>(std::floor(y)), mapHeight - 1));
            int y2 = std::max(0, std::min(static_cast<int>(std::ceil(y)), mapHeight - 1));

            double q11 = depthMap[y1][x1];
            double q12 = depthMap[y2][x1];
            double q21 = depthMap[y1][x2];
            double q22 = depthMap[y2][x2];

            double interpolatedValue = bilinearInterpolation(x, y, q11, q12, q21, q22, x1, x2, y1, y2);
            int gridIdx = grid->index(i, j);
            depthField->setValue(gridIdx, interpolatedValue);
        }
    }
}

#endif
