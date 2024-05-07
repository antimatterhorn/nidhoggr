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
    NodeList() {}

    NodeList(int numNodes) {  
        _ids = Field<int>("id");     
        for (int i=0; i<numNodes; ++i) { _ids.addValue(i); }
        addField(std::make_shared<Field<int>>(_ids));
    }

    ~NodeList() {
        // Clean up memory for extra fields
        // for (auto fieldPtr : _extraFields) {
        //     delete fieldPtr;
        // }
    }

    void addField(const std::shared_ptr<FieldBase>& fieldPtr) {
        _fields.push_back(fieldPtr);
    }

    template <typename T>
    void insertField(const std::string& name) {
        auto newField = std::make_shared<Field<T>>(name, this->size());
        _extraFields.push_back(newField); // Use make_shared for field creation
        addField(newField); // Add pointer to the new field to _fields
    }

    void addNodeList(const NodeList& other) {
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

    // std::vector<FieldBase*> 
    // getFields() { return _fields; }

    template <typename T>
    Field<T>* getFieldByName(const Name& name) const {
        for (const auto& fieldPtr : _fields) {
            if (fieldPtr->hasName() && fieldPtr->getNameString() == name.name()) {
                Field<T>* castedField = dynamic_cast<Field<T>*>(fieldPtr.get());
                if (castedField != nullptr) {
                    return castedField; // Return the field if found and correctly casted
                }
            }
        }
        return nullptr; // Return nullptr if no matching field is found
    }


    template <typename T>
    Field<T>* 
    getField(const std::string& name) const {
        return getFieldByName<T>(Name(name));
    }

    Field<double>*
    mass() const {
        return getFieldByName<double>(Name("mass")); 
    }

    template <int dim>
    Field<Lin::Vector<dim>>*
    velocity() const {
        for (const auto& fieldPtr : _fields) {
            if (fieldPtr->hasName() && fieldPtr->getName() == Name("velocity")) {
                Field<Lin::Vector<dim>>* velocityField = dynamic_cast<Field<Lin::Vector<dim>>*>(fieldPtr.get());
                if (velocityField != nullptr) {
                    return velocityField; // Return the velocity field if found
                }
            }
        }
        return nullptr; 
    }

    template <int dim>
    Field<Lin::Vector<dim>>*
    position() const {
        for (const auto& fieldPtr : _fields) {
            if (fieldPtr->hasName() && fieldPtr->getName() == Name("position")) {
                Field<Lin::Vector<dim>>* positionField = dynamic_cast<Field<Lin::Vector<dim>>*>(fieldPtr.get());
                if (positionField != nullptr) {
                    return positionField; // Return the position field if found
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