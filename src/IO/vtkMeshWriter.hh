#ifndef VTKMESHWRITER_HH
#define VTKMESHWRITER_HH

#include <vector>
#include <string>
#include <fstream>
#include "../DataBase/nodeList.hh"
#include "../Math/vectorMath.hh"

template <int dim>
class VTKMeshWriter {
public:
    VTKMeshWriter(const std::string& baseName, const NodeList* nodeList, const std::vector<std::string>& fieldNames);

    void write(const std::string& fileName);

private:
    std::string baseName;
    const NodeList* nodeList;
    std::vector<std::string> fieldNames;

    void writeVTKHeader(std::ofstream& outFile);
    void writePointCoordinates(std::ofstream& outFile);
    void writePointData(std::ofstream& outFile);
};

#include "vtkMeshWriter.cc"

#endif // VTWMESHWRITER_HH