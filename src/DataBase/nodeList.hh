#ifndef NODELIST_HH
#define NODELIST_HH

#include <vector>
#include <string>
#include "field.hh"
#include "../Type/name.hh"

class NodeList {
private:
    std::vector<FieldBase*> fields;
    Field<int> ids;
public:
    NodeList() {}

    NodeList(int numNodes) {  
        ids = Field<int>("id");     
        for (int i=0; i<numNodes;++i) { ids.addValue(i); }
        addField(&ids);
    }

    void 
    addField(FieldBase* fieldPtr) {
        fields.emplace_back(fieldPtr);
    }

    void 
    addNodeList(const NodeList& other) {
        fields.insert(fields.end(), other.fields.begin(), other.fields.end());
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
    getFields() { return fields; }

    FieldBase* 
    getFieldByName(const Name& name) const {
        for (FieldBase* field : fields) {
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

    size_t 
    getFieldCount() const {
        return fields.size();
    }

    size_t 
    getNumNodes() const {
        return fields[0]->getSize();
    }

    std::vector<std::string> 
    fieldNames() const {
        std::vector<std::string> names;
        for (const auto& field : fields) {
            // Check if the pointer is valid and the FieldList has a name
            if (field && field->hasName()) {
                names.push_back(field->getNameString());
            }
        }
        return names;
    }

    Field<int>& 
    nodes() {
        return ids;
    }

    unsigned int 
    size() const { return getNumNodes(); }

    ~NodeList() {
        // for (auto field : fields) {
        //     delete field;
        // }
    }
};

#endif // NODELIST_HH