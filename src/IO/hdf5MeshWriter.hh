#ifndef HDF5MESHWRITER_HH
#define HDF5MESHWRITER_HH

#include <vector>
#include <string>
#include <H5Cpp.h>
#include "../DataBase/nodeList.hh"
#include "../Math/vectorMath.hh"

template <int dim>
class HDF5MeshWriter {
public:
    HDF5MeshWriter(const std::string& baseName, const NodeList* nodeList, const std::vector<std::string>& fieldNames);

    void write(const std::string& fileName);

private:
    std::string baseName;
    const NodeList* nodeList;
    std::vector<std::string> fieldNames;

    void writeCellCenterData(H5::H5File& file);
};

#include "hdf5MeshWriter.cc"

#endif // HDF5MESHWRITER_HH
