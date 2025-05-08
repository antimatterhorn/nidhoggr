#ifndef SILOMESHWRITER_CC
#define SILOMESHWRITER_CC

#include "siloMeshWriter.hh"

template <int dim>
SiloMeshWriter<dim>::SiloMeshWriter(const std::string& baseName, const NodeList& nodeList, const std::vector<std::string>& fieldNames)
    : baseName(baseName), nodeList(nodeList), fieldNames(fieldNames) {}

template <int dim>
void 
SiloMeshWriter<dim>::write(const std::string& fileName, const bool& isGrid) {
    std::string fullFileName = baseName + fileName;
    DBfile *dbfile = DBCreate(fullFileName.c_str(), DB_CLOBBER, DB_LOCAL, "Simulation data", DB_PDB);

    if (!dbfile) {
        std::cerr << "Error creating Silo file!" << std::endl;
        return;
    }


    if (isGrid) {
        writeStructuredMesh(dbfile);
        writeMeshFields(dbfile);
    } else {
        writePointMesh(dbfile);
        writeFields(dbfile);
    }

    DBClose(dbfile);
}

template <int dim>
void SiloMeshWriter<dim>::writeMeshFields(DBfile* dbfile) {
    // Get zone dimensions (requires NodeList to report full grid shape)
    std::array<unsigned int, dim> fullShape = nodeList.gridShape();  // e.g., {nx, ny}
    std::array<int, dim> zoneDims;
    for (int d = 0; d < dim; ++d) {
        zoneDims[d] = static_cast<int>(fullShape[d] - 1);
    }

    for (const auto& field_name : fieldNames) {
        if (field_name == "position") continue;

        auto field_double = nodeList.getField<double>(field_name);
        if (field_double) {
            std::vector<float> field_data(field_double->size());
            for (unsigned int i = 0; i < field_double->size(); ++i) {
                field_data[i] = static_cast<float>(field_double->getValue(i));
            }

            DBPutQuadvar1(dbfile,
                          field_name.c_str(),
                          "quadmesh",
                          field_data.data(),
                          zoneDims.data(),
                          dim,
                          DB_FLOAT,
                          DB_ZONECENT,
                          nullptr);
            continue;
        }

        auto field_vector = nodeList.getField<Lin::Vector<dim>>(field_name);
        if (field_vector) {
            std::vector<float> field_data[dim];
            for (int d = 0; d < dim; ++d) {
                field_data[d].resize(field_vector->size());
            }

            for (unsigned int i = 0; i < field_vector->size(); ++i) {
                Lin::Vector<dim> vec = field_vector->getValue(i);
                for (int d = 0; d < dim; ++d) {
                    field_data[d][i] = static_cast<float>(vec[d]);
                }
            }

            float* field_data_ptr[dim];
            for (int d = 0; d < dim; ++d) {
                field_data_ptr[d] = field_data[d].data();
            }

            DBPutQuadvar(dbfile,
                         field_name.c_str(),
                         "quadmesh",
                         dim,
                         field_data_ptr,
                         zoneDims.data(),
                         DB_FLOAT,
                         DB_ZONECENT,
                         nullptr);
        }
    }
}



template <int dim>
void 
SiloMeshWriter<dim>::writePointMesh(DBfile* dbfile) {
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
        Lin::Vector<dim> pos = positions->getValue(idx);
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
SiloMeshWriter<dim>::writeFields(DBfile* dbfile) {
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
                Lin::Vector<dim> vec = field_vector->getValue(i);
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

template <int dim>
void SiloMeshWriter<dim>::writeStructuredMesh(DBfile* dbfile) {
    // Retrieve grid shape and spacing
    std::array<unsigned int, dim> shape = nodeList.gridShape();    // e.g., {nx, ny[, nz]}
    std::array<float, dim> spacing = nodeList.gridSpacing();       // e.g., {dx, dy[, dz]}

    // Construct coordinate arrays for each axis
    std::vector<float> coords[dim];
    for (int d = 0; d < dim; ++d) {
        coords[d].resize(shape[d]);
        for (unsigned int i = 0; i < shape[d]; ++i) {
            coords[d][i] = i * spacing[d];
        }
    }

    // Create pointer array to pass to Silo
    float* coord_ptrs[dim];
    for (int d = 0; d < dim; ++d) {
        coord_ptrs[d] = coords[d].data();
    }

    int int_shape[dim];
    for (int d = 0; d < dim; ++d) {
        int_shape[d] = static_cast<int>(shape[d]);
    }

    DBPutQuadmesh(dbfile,
                  "quadmesh",
                  nullptr,         // Use default axis names
                  coord_ptrs,      // Coordinate pointers
                  int_shape,       // Number of nodes per dimension
                  dim,
                  DB_FLOAT,
                  DB_COLLINEAR,    // Grid is rectilinear and aligned
                  nullptr);
}


#endif