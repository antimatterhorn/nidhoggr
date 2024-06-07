#ifndef IMPORTDEPTHMAP_HH
#define IMPORTDEPTHMAP_HH

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include "../Math/vectorMath.hh"
#include "../DataBase/field.hh"
#include "../Mesh/grid.hh"

class ImportDepthMap {
public:
    std::vector<std::vector<double>> depthMap;

    ImportDepthMap(const std::string& filename);
    void populateDepthField(Mesh::Grid<2>* grid);

private:
    std::vector<std::vector<double>> loadDepthMap(const std::string& filename);
    double bilinearInterpolation(double x, double y,
                                 double q11, double q12, double q21, double q22,
                                 double x1, double x2, double y1, double y2);
};

#include "importDepthMap.cc"
#endif // IMPORTDEPTHMAP_HH
