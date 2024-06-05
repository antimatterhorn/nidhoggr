#ifndef HDF5MESHWRITER_CC
#define HDF5MESHWRITER_CC

#include "hdf5MeshWriter.hh"

template <int dim>
HDF5MeshWriter<dim>::HDF5MeshWriter(const std::string& baseName, const NodeList* nodeList, const std::vector<std::string>& fieldNames)
    : baseName(baseName), nodeList(nodeList), fieldNames(fieldNames) {}

template <int dim>
void 
HDF5MeshWriter<dim>::write(const std::string& fileName) {
    std::string fullFileName = baseName + fileName;
    H5::H5File file(fullFileName, H5F_ACC_TRUNC);

    writePointData(file);

    file.close();
}

template <int dim>
void 
HDF5MeshWriter<dim>::writePointData(H5::Group& group) {
    auto positions = nodeList->position<dim>();
    if (!positions) {
        std::cerr << "Position field is not available in the NodeList!" << std::endl;
        return;
    }

    hsize_t dims[2] = {nodeList->size(), dim};
    H5::DataSpace dataspace(2, dims);

    H5::DataSet dataset = group.createDataSet("Position", H5::PredType::NATIVE_DOUBLE, dataspace);

    std::vector<double> data(nodeList->size() * dim);
    for (unsigned int idx = 0; idx < nodeList->size(); ++idx) {
        Lin::Vector<dim> pos = positions->getValue(idx);
        for (int i = 0; i < dim; ++i) {
            data[idx * dim + i] = pos[i];
        }
    }

    dataset.write(data.data(), H5::PredType::NATIVE_DOUBLE);
}

#endif