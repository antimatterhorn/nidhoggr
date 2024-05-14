#ifndef STATE_HH
#define STATE_HH

#include "../DataBase/nodeList.hh"
#include "../Type/name.hh"

class State {
private:   
    NodeList* nodeList;
    NodeList derivs;
public:

    std::vector<FieldBase*> fields;
    std::vector<FieldBase*> dfields;
    
    State(NodeList* nodeList) : 
        nodeList(nodeList) {
            derivs = NodeList(nodeList->size());
        };
    
    State(State* other) : nodeList(other->derivatives()){
        for (FieldBase* field : other->dfields)
            fields.push_back(field);
    }

    ~State() {};

    template <typename T>
    void
    enrollField(Field<T>* field) {
        fields.push_back(field);
    }

    template <typename T>
    Field<T>* getFieldByName(const Name& name) const {
        for (FieldBase* field : fields) {
            if (field->hasName() && field->getNameString() == name.name()) {
                Field<T>* castedField = dynamic_cast<Field<T>*>(field);
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

    template <typename T>
    void
    insertDeriv(const std::string& name) {
        derivs.insertField<T>(name);
        dfields.push_back(this->getField<T>(name));
    }

    template <typename T>
    Field<T>* getDerivative(const std::string& name) {
        return derivs.getField<T>(name);
    }

    NodeList*
    derivatives() { return &derivs; }

    NodeList*
    getNodeList() { return nodeList; }
};

#endif //STATE_HH