#include <vector>
#include <string>
#include "../DataBase/field.cc" 

class State {
private:
    std::vector<FieldListBase*> fieldLists;
public:
    State() {}

    void addFieldList(FieldListBase* fieldListPtr) {
        fieldLists.push_back(fieldListPtr);
    }

    void addState(const State& other) {
        fieldLists.insert(fieldLists.end(), other.fieldLists.begin(), other.fieldLists.end());
    }

    State operator+(const State& other) const {
        State newState = *this; 
        newState.addState(other);
        return newState;
    }

    State& operator+=(const State& other) {
        addState(other);
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

    ~State() {
        for (auto fieldList : fieldLists) {
            delete fieldList;
        }
    }
};
