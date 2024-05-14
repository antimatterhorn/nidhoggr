#ifndef STATE_HH
#define STATE_HH

#include "../DataBase/nodeList.hh"
#include "../Type/name.hh"

class State {
private:   
    std::vector<std::shared_ptr<FieldBase>> fields;
    int numNodes;
public:
    State(int numNodes) : 
        numNodes(numNodes) { };

    ~State() {};

    template <typename T>
    Field<T>* getFieldByName(const Name& name) const {
        for (const auto& fieldPtr : fields) {
            if (fieldPtr->hasName() && fieldPtr->getNameString() == name.name()) {
                Field<T>* castedField = dynamic_cast<Field<T>*>(fieldPtr.get());
                if (castedField != nullptr) {
                    return castedField; // Return the field if found and correctly casted
                }
            }
        }
        return nullptr; // Return nullptr if no matching field is found
    }

    void addField(const std::shared_ptr<FieldBase>& fieldPtr) {
        fields.push_back(fieldPtr);
    }

    template <typename T>
    Field<T>* 
    getField(const std::string& name) const {
        return getFieldByName<T>(Name(name));
    }
    
    template <typename T>
    void insertField(const std::string& name) {
        auto newField = std::make_shared<Field<T>>(name, this->size());
        fields.push_back(newField); // Use make_shared for field creation
        addField(newField); // Add pointer to the new field to _fields
    }

    int
    size() const { return numNodes; }

    int
    count() const { return fields.size(); }

    State
    operator*(const State& other) {
        
    }
};

#endif //STATE_HH