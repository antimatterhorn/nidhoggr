#ifndef NODELIST_HH
#define NODELIST_HH

#include <vector>
#include <string>
#include "field.hh"
#include "../Type/name.hh"
#include "../Math/vectorMath.hh"

class NodeList {
private:
    std::vector<FieldBase*> _fields;
    Field<int> _ids;
    std::vector<FieldBase*> _extraFields;
public:
    NodeList() {}

    NodeList(int numNodes) {  
        _ids = Field<int>("id");     
        for (int i=0; i<numNodes; ++i) { _ids.addValue(i); }
        addField(&_ids);
    }

    ~NodeList() {
        // Clean up memory for extra fields
        for (auto fieldPtr : _extraFields) {
            delete fieldPtr;
        }
    }

    void addField(FieldBase* fieldPtr) {
        _fields.emplace_back(fieldPtr);
    }

    template <typename T>
    void insertField(std::string name) {
        Field<T>* newField = new Field<T>(name, this->size());
        _extraFields.emplace_back(newField); // Store pointer to the new field
        addField(newField); // Add pointer to the new field to _fields
    }

    void 
    addNodeList(const NodeList& other) {
        _fields.insert(_fields.end(), other._fields.begin(), other._fields.end());
    }

    NodeList 
    operator+(const NodeList& other) const {
        NodeList newNodeList = *this; 
        newNodeList.addNodeList(other);
        return newNodeList;
    }

    NodeList& 
    operator+=(const NodeList& other) {
        addNodeList(other);
        return *this;
    }

    std::vector<FieldBase*> 
    getFields() { return _fields; }

    FieldBase* 
    getFieldByName(const Name& name) const {
        for (FieldBase* field : _fields) {
            if (field->hasName() && field->getName() == name) {
                return field;
            }
        }
        return nullptr; // Return nullptr if no matching FieldList is found
    }

    FieldBase* 
    getField(const std::string& name) const {
        return getFieldByName(Name(name));
    }

    Field<double>*
    mass() const {
        for (FieldBase* field : _fields) {
            if (field->hasName() && field->getName() == Name("mass")) {
                Field<double>* massField = dynamic_cast<Field<double>*>(field);
                if (massField != nullptr) {
                    return massField; // Return the mass field if found
                }
            }
        }
        return nullptr; 
    }

    template <int dim>
    Field<Lin::Vector<dim>>*
    velocity() const {
        for (FieldBase* field : _fields) {
            if (field->hasName() && field->getName() == Name("velocity")) {
                Field<Lin::Vector<dim>>* velocityField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                if (velocityField != nullptr) {
                    return velocityField; // Return the mass field if found
                }
            }
        }
        return nullptr; 
    }

    template <int dim>
    Field<Lin::Vector<dim>>*
    position() const {
        for (FieldBase* field : _fields) {
            if (field->hasName() && field->getName() == Name("position")) {
                Field<Lin::Vector<dim>>* positionField = dynamic_cast<Field<Lin::Vector<dim>>*>(field);
                if (positionField != nullptr) {
                    return positionField; // Return the mass field if found
                }
            }
        }
        return nullptr; 
    }

    size_t 
    getFieldCount() const {
        return _fields.size();
    }

    size_t 
    getNumNodes() const {
        return _fields[0]->getSize();
    }

    std::vector<std::string> 
    fieldNames() const {
        std::vector<std::string> names;
        for (const auto& field : _fields) {
            // Check if the pointer is valid and the FieldList has a name
            if (field && field->hasName()) {
                names.push_back(field->getNameString());
            }
        }
        return names;
    }

    Field<int>& 
    nodes() {
        return _ids;
    }

    unsigned int 
    size() const { return getNumNodes(); }

};

#endif // NODELIST_HH