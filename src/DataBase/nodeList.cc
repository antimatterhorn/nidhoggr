#include <vector>
#include <string>

class NodeList {
private:
    std::vector<FieldListBase*> fieldLists;
public:
    NodeList() {}

    void addFieldList(FieldListBase* fieldListPtr) {
        fieldLists.push_back(fieldListPtr);
    }

    void addNodeList(const NodeList& other) {
        fieldLists.insert(fieldLists.end(), other.fieldLists.begin(), other.fieldLists.end());
    }

    NodeList operator+(const NodeList& other) const {
        NodeList newNodeList = *this; 
        newNodeList.addNodeList(other);
        return newNodeList;
    }

    NodeList& operator+=(const NodeList& other) {
        addNodeList(other);
        return *this;
    }

    std::vector<FieldListBase*> getFieldLists() { return fieldLists; }

    FieldListBase* getFieldList(size_t index) {
        return fieldLists[index];
    }

    FieldListBase* getFieldList(const std::string& name) const {
        for (FieldListBase* fieldList : fieldLists) {
            if (fieldList->hasName() && fieldList->getName() == name) {
                return fieldList;
            }
        }
        return nullptr; // Return nullptr if no matching FieldList is found
    }

    size_t getFieldListCount() const {
        return fieldLists.size();
    }

    size_t getNumNodes() const {
        return fieldLists[0]->getSize();
    }

    std::vector<std::string> fieldNames() const {
        std::vector<std::string> names;
        for (const auto& fieldList : fieldLists) {
            // Check if the pointer is valid and the FieldList has a name
            if (fieldList && fieldList->hasName()) {
                names.push_back(fieldList->getName());
            }
        }
        return names;
    }

    ~NodeList() {
        for (auto fieldList : fieldLists) {
            delete fieldList;
        }
    }
};
