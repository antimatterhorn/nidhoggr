#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include "../Math/vectorMath.hh"

std::pair<std::vector<Lin::Vector<2>>, std::vector<std::vector<int>>> importObj2d(const std::string& file_path, const std::string& axes="(x,z)") {
    assert(axes == "(x,y)" || axes == "(x,z)" || axes == "(y,z)");

    std::vector<Lin::Vector<2>> vertices;
    std::vector<std::vector<int>> faces;

    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string part;
        ss >> part;

        if (part.empty())
            continue;

        if (part == "v") {
            double x, y, z;
            ss >> x >> y >> z;
            if (axes == "(x,z)")
                vertices.push_back({x, z});
            else if (axes == "(x,y)")
                vertices.push_back({x, y});
            else
                vertices.push_back({y, z});
        } else if (part == "f") {
            std::vector<int> face;
            while (ss >> part) {
                int index = std::stoi(part.substr(0, part.find('/')));
                face.push_back(index);
            }
            faces.push_back(face);
        }
    }

    return std::make_pair(vertices, faces);