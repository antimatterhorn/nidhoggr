#ifndef STATE_HH
#define STATE_HH

#include "../DataBase/nodeList.hh"
#include "../Type/name.hh"
#include "../Math/vectorMath.hh"

template <int dim>
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

    template <typename T>
    void 
    addField(const Field<T>& fieldPtr) {
        fields.push_back(fieldPtr);
    }

    template <typename T>
    Field<T>* 
    getField(const std::string& name) const {
        return getFieldByName<T>(Name(name));
    }
    
    template <typename T>
    void 
    insertField(const std::string& name) {
        auto newField = std::make_shared<Field<T>>(name, this->size());
        fields.push_back(newField); // Use make_shared for field creation
        addField(newField); // Add pointer to the new field to _fields
    }

    FieldBase* 
    getFieldByIndex(int index) const {
        if (index < 0 || index >= fields.size()) {
            return nullptr; // Return nullptr if index is out of range
        }
        return fields[index].get(); // Return the raw pointer to the field at index
    }

    int
    size() const { return numNodes; }

    int
    count() const { return fields.size(); }

    State& 
    operator+=(const State& other) {
        if (this != &other) {
            if (this->count() != other.count() || this->size() != other.size()) {
                throw std::invalid_argument("Incompatible State objects for addition");
            }
            for (const auto& fieldPtr : fields) {
                if (auto* doubleField = dynamic_cast<Field<double>*>(fieldPtr.get())) {
                    auto* otherDoubleField = dynamic_cast<const Field<double>*>(other.getField<double>(doubleField->getNameString()));
                    if (otherDoubleField) {
                        *doubleField += *otherDoubleField;
                    }
                } else if (auto* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(fieldPtr.get())) {
                    auto* otherVectorField = dynamic_cast<const Field<Lin::Vector<dim>>*>(other.getField<Lin::Vector<dim>>(vectorField->getNameString()));
                    if (otherVectorField) {
                        *vectorField += *otherVectorField;
                    }
                }
            }
        }
        return *this;
    }

    // Implement *= operator for State class
    State& 
    operator*=(const double other) {
        for (const auto& fieldPtr : fields) {
            if (auto* doubleField = dynamic_cast<Field<double>*>(fieldPtr.get())) {
                *doubleField *= other;
            } else if (auto* vectorField = dynamic_cast<Field<Lin::Vector<dim>>*>(fieldPtr.get())) {
                *vectorField *= other;
            }
        }
        return *this;
    }

    // Overload the = assignment operator to copy the fields from another state
    State& 
    operator=(const State& rhs) {
        fields = rhs.fields;
        numNodes = rhs.numNodes;
        return *this;
    }
};


#endif //STATE_HH