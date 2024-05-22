#ifndef NODELIST_HH
#define NODELIST_HH

#include <vector>
#include <string>
#include <memory> // Include for std::shared_ptr
#include "field.hh"
#include "../Type/name.hh"
#include "../Math/vectorMath.hh"

class NodeList {
private:
    std::vector<std::shared_ptr<FieldBase>> _fields;
    Field<int> _ids;
    std::vector<std::shared_ptr<FieldBase>> _extraFields;

public:
    NodeList();
    NodeList(int numNodes);
    ~NodeList();

    void addField(const std::shared_ptr<FieldBase>& fieldPtr);

    template <typename T>
    void insertField(const std::string& name);

    void addNodeList(const NodeList& other);

    NodeList operator+(const NodeList& other) const;
    NodeList& operator+=(const NodeList& other);

    template <typename T>
    Field<T>* getFieldByName(const Name& name) const;

    template <typename T>
    Field<T>* getField(const std::string& name) const;

    Field<double>* mass() const;

    template <int dim>
    Field<Lin::Vector<dim>>* velocity() const;

    template <int dim>
    Field<Lin::Vector<dim>>* position() const;

    size_t getFieldCount() const;
    size_t getNumNodes() const;
    std::vector<std::string> fieldNames() const;
    Field<int>& nodes();
    unsigned int size() const;
};

#include "nodeList.cc" // Include the template implementation

#endif // NODELIST_HH
