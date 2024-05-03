#include "../DataBase/nodeList.hh"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

void 
export_to_vtk(const std::string& file_path, const NodeList& nodeList) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << file_path << std::endl;
        return;
    }

    file << "# vtk DataFile Version 3.0\n";
    file << "NidHoggr Exported for VisIt\n";
    file << "ASCII\n";
    file << "DATASET UNSTRUCTURED_GRID\n";

    // Write node coordinates
    file << "POINTS " << nodeList.getNumNodes() << " double\n";
    for (const auto& node : nodeList.nodes()) {
        for (size_t i = 0; i < node.coordinates.size(); ++i) {
            file << node.coordinates[i] << " ";
        }
        if (node.coordinates.size() == 2) {
            file << "0.0\n";
        } else if (node.coordinates.size() == 3) {
            file << "\n";
        }
    }

    // Write element connectivity
    size_t total_nodes = 0;
    for (const auto& element : nodeList.elements) {
        total_nodes += element.nodes.size();
    }
    file << "CELLS " << nodeList.elements.size() << " " << nodeList.elements.size() + total_nodes << "\n";
    for (const auto& element : nodeList.elements) {
        file << element.nodes.size() << " ";
        for (const auto& node : element.nodes) {
            file << node.id - 1 << " ";
        }
        file << "\n";
    }

    file << "CELL_TYPES " << nodeList.elements.size() << "\n";
    for (const auto& element : nodeList.elements) {
        if (element.nodes.size() == 3) {
            file << "5 ";  // VTK_TRIANGLE
        } else if (element.nodes.size() == 4) {
            file << "9 ";  // VTK_QUAD
        }
        // Add more cases for different element types as needed
    }
    file << "\n";

    // Point Data
    file << "POINT_DATA " << nodeList.nodes.size() << "\n";
    file << "SCALARS x double 1\n";
    file << "LOOKUP_TABLE default\n";
    for (const auto& node : nodeList.nodes) {
        file << node.coordinates[0] << "\n";
    }
    file << "\n";

    file << "SCALARS y double 1\n";
    file << "LOOKUP_TABLE default\n";
    for (const auto& node : nodeList.nodes) {
        file << node.coordinates[1] << "\n";
    }
    file << "\n";

    // Face Data
    file << "CELL_DATA " << nodeList.elements.size() << "\n";
    file << "SCALARS id int 1\n";
    file << "LOOKUP_TABLE default\n";
    for (const auto& element : nodeList.elements) {
        file << element.id << "\n";
    }
    file << "\n";

    file << "SCALARS area double 1\n";
    file << "LOOKUP_TABLE default\n";
    for (const auto& element : nodeList.elements) {
        // Assuming `area` is a member of the Element struct
        file << element.area << "\n";
    }
    file << "\n";
}