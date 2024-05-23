#ifndef SILOWRITER_HH
#define SILOWRITER_HH

#include <silo.h>
#include <vector>
#include <string>
#include <iostream>
#include "../DataBase/nodeList.hh"
#include "../Math/vectorMath.hh"

template <int dim>
class SiloWriter {
public:
    SiloWriter(const std::string& baseName, const NodeList& nodeList, const std::vector<std::string>& fieldNames);

    void write(const std::string& fileName);

private:
    std::string baseName;
    const NodeList& nodeList;
    std::vector<std::string> fieldNames;

    void writePointMesh(DBfile* dbfile);
    void writeFields(DBfile* dbfile);
};

#include "siloWriter.cc"

#endif // SILOWRITER_HH
