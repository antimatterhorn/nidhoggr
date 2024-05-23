#ifndef VTKMESHWRITER_CC
#define VTKMESHWRITER_CC

#include "vtkMeshWriter.hh"
#include <iomanip> // for std::setprecision

template <int dim>
VTKMeshWriter<dim>::VTKMeshWriter(const std::string& baseName, const NodeList* nodeList, const std::vector<std::string>& fieldNames)
    : baseName(baseName), nodeList(nodeList), fieldNames(fieldNames) {}

template <int dim>
void 
VTKMeshWriter<dim>::write(const std::string& fileName) {
    std::string fullFileName = baseName + fileName;
    std::ofstream outFile(fullFileName);
    if (!outFile.is_open()) {
        std::cerr << "Error creating VTK file!" << std::endl;
        return;
    }

    writeVTKHeader(outFile);
    writePointCoordinates(outFile);
    writePointData(outFile);

    outFile.close();
}

template <int dim>
void 
VTKMeshWriter<dim>::writeVTKHeader(std::ofstream& outFile) {
    outFile << "# vtk DataFile Version 3.0\n";
    outFile << "VTK file created by VTKMeshWriter\n";
    outFile << "ASCII\n";
    outFile << "DATASET UNSTRUCTURED_GRID\n";
}

template <int dim>
void 
VTKMeshWriter<dim>::writePointCoordinates(std::ofstream& outFile) {
    outFile << "POINTS " << nodeList->size() << " float\n";
    auto positions = nodeList->position<dim>();
    if (!positions) {
        std::cerr << "Position field is not available in the NodeList!" << std::endl;
        return;
    }
    for (unsigned int idx = 0; idx < nodeList->size(); ++idx) {
        Lin::Vector<dim> pos = positions->getValue(idx);
        for (int i = 0; i < dim; ++i) {
            outFile << pos[i] << " ";
        }
        if (dim < 3)
            outFile << "0\n";  // Append 0 for the z-coordinate if dim < 3
    }
}

template <int dim>
void 
VTKMeshWriter<dim>::writePointData(std::ofstream& outFile) {
    outFile << "POINT_DATA " << nodeList->size() << "\n";
    for (const auto& field_name : fieldNames) {
        if (field_name == "position") continue;  // Skip the position field

        auto field = nodeList->getField<double>(field_name);
        if (field) {
            outFile << "SCALARS " << field_name << " float\n";
            outFile << "LOOKUP_TABLE default\n";
            for (unsigned int i = 0; i < field->size(); ++i) {
                outFile << field->getValue(i) << "\n";
            }
            continue;
        }

        auto field_vector = nodeList->getField<Lin::Vector<dim>>(field_name);
        if (field_vector) {
            for (int d = 0; d < dim; ++d) {
                outFile << "SCALARS " << field_name << "_" << d << " float\n";
                outFile << "LOOKUP_TABLE default\n";
                for (unsigned int i = 0; i < field_vector->size(); ++i) {
                    outFile << field_vector->getValue(i)[d] << "\n";
                }
            }
        } else {
            std::cerr << "Field " << field_name << " not found in NodeList!" << std::endl;
        }
    }
}

#endif
