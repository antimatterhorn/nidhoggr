#ifndef SILOWRITER_CC
#define SILOWRITER_CC

#include "siloWriter.hh"

template <int dim>
SiloWriter<dim>::SiloWriter(const std::string& baseName, const NodeList& nodeList, const std::vector<std::string>& fieldNames)
    : baseName(baseName), nodeList(nodeList), fieldNames(fieldNames) {}

template <int dim>
void 
SiloWriter<dim>::write(const std::string& fileName) {
    std::string fullFileName = baseName + fileName;
    DBfile *dbfile = DBCreate(fullFileName.c_str(), DB_CLOBBER, DB_LOCAL, "Simulation data", DB_HDF5);

    if (!dbfile) {
        std::cerr << "Error creating Silo file!" << std::endl;
        return;
    }

    writePointMesh(dbfile);
    writeFields(dbfile);

    DBClose(dbfile);
}

template <int dim>
void 
SiloWriter<dim>::writePointMesh(DBfile* dbfile) {
    auto positions = nodeList.position<dim>();
    if (!positions) {
        std::cerr << "Position field is not available in the NodeList!" << std::endl;
        return;
    }

    std::vector<float> coords[dim];
    for (int i = 0; i < dim; ++i) {
        coords[i].resize(nodeList.size());
    }

    for (unsigned int idx = 0; idx < nodeList.size(); ++idx) {
        Lin::Vector<dim> pos = positions->value(idx);
        for (int i = 0; i < dim; ++i) {
            coords[i][idx] = static_cast<float>(pos[i]);
        }
    }

    float *coords_ptr[dim];
    for (int i = 0; i < dim; ++i) {
        coords_ptr[i] = coords[i].data();
    }

    DBPutPointmesh(dbfile, "pointmesh", dim, coords_ptr, nodeList.size(), DB_FLOAT, nullptr);
}

template <int dim>
void 
SiloWriter<dim>::writeFields(DBfile* dbfile) {
    for (const auto& field_name : fieldNames) {
        if (field_name == "position") continue;  // Skip the position field

        auto field_double = nodeList.getField<double>(field_name);
        if (field_double) {
            std::vector<float> field_data(nodeList.size());
            for (unsigned int i = 0; i < field_double->size(); ++i) {
                field_data[i] = static_cast<float>(field_double->getValue(i));
            }
            DBPutPointvar1(dbfile, field_name.c_str(), "pointmesh", field_data.data(), nodeList.size(), DB_FLOAT, nullptr);
            continue;
        }

        auto field_vector = nodeList.getField<Lin::Vector<dim>>(field_name);
        if (field_vector) {
            std::vector<float> field_data[dim];
            for (int d = 0; d < dim; ++d) {
                field_data[d].resize(nodeList.size());
            }
            for (unsigned int i = 0; i < field_vector->size(); ++i) {
                Lin::Vector<dim> vec = field_vector->value(i);
                for (int d = 0; d < dim; ++d) {
                    field_data[d][i] = static_cast<float>(vec[d]);
                }
            }

            float *field_data_ptr[dim];
            for (int d = 0; d < dim; ++d) {
                field_data_ptr[d] = field_data[d].data();
            }

            DBPutPointvar(dbfile, field_name.c_str(), "pointmesh", dim, field_data_ptr, nodeList.size(), DB_FLOAT, nullptr);
        }
    }
}

#endif