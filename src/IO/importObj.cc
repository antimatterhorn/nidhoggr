#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include "../Math/vectorMath.hh"
#include "../DataBase/field.hh"

// template <int dim>
// std::pair<Field<Lin::Vector<dim>>, Field<std::vector<int>>> 
// importObj(const std::string& file_path, const std::string& axes="(x,z)") {
//     if(dim==2)
//         return importObj2d(file_path,axes);
//     else if(dim==3)
//         return importObj3d(file_path);
// }

std::pair<Field<Lin::Vector<2>>, Field<std::vector<int>>> 
importObj2d(const std::string& file_path, const std::string& axes="(x,z)") {
    assert(axes == "(x,y)" || axes == "(x,z)" || axes == "(y,z)");

    Field<Lin::Vector<2>> vertices("vertices");
    Field<std::vector<int>> faces("faces");

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
                vertices.addValue({x, z});
            else if (axes == "(x,y)")
                vertices.addValue({x, y});
            else
                vertices.addValue({y, z});
        } else if (part == "f") {
            std::vector<int> face;
            while (ss >> part) {
                int index = std::stoi(part.substr(0, part.find('/')));
                face.push_back(index);
            }
            faces.addValue(face);
        }
    }

    return std::make_pair(vertices, faces);
}

std::pair<Field<Lin::Vector<3>>, Field<std::vector<int>>> 
importObj3d(const std::string& file_path) {

}