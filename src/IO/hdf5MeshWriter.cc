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

    writeCellCenterData(file);

    file.close();
}

template <int dim>
void 
HDF5MeshWriter<dim>::writeCellCenterData(H5::H5File& file) {
    auto positions = nodeList->position<dim>();
    if (!positions) {
        std::cerr << "Position field is not available in the NodeList!" << std::endl;
        return;
    }

    // Write cell center positions
    hsize_t dims[2] = {nodeList->size(), dim};
    H5::DataSpace dataspace(2, dims);
    H5::DataSet dataset = file.createDataSet("CellCenters", H5::PredType::NATIVE_DOUBLE, dataspace);

    std::vector<double> data(nodeList->size() * dim);
    for (unsigned int idx = 0; idx < nodeList->size(); ++idx) {
        Lin::Vector<dim> pos = positions->getValue(idx);
        for (int i = 0; i < dim; ++i) {
            data[idx * dim + i] = pos[i];
        }
    }
    dataset.write(data.data(), H5::PredType::NATIVE_DOUBLE);

    // Write field data at cell centers
    for (const auto& fieldName : fieldNames) {
        auto field = nodeList->getField<double>(fieldName);
        if (field) {
            hsize_t field_dims[1] = {nodeList->size()};
            H5::DataSpace field_dataspace(1, field_dims);
            H5::DataSet field_dataset = file.createDataSet(fieldName, H5::PredType::NATIVE_DOUBLE, field_dataspace);

            std::vector<double> field_data(nodeList->size());
            for (unsigned int i = 0; i < nodeList->size(); ++i) {
                field_data[i] = field->getValue(i);
            }
            field_dataset.write(field_data.data(), H5::PredType::NATIVE_DOUBLE);
        } else {
            std::cerr << "Field " << fieldName << " not found in NodeList!" << std::endl;
        }
    }
}

#endif // HDF5MESHWRITER_CC
