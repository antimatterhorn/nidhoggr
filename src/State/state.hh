#ifndef STATE_HH
#define STATE_HH

#include "../DataBase/nodeList.hh"
#include "../Type/name.hh"

class State {
private:
    std::vector<FieldBase*> fields;
    std::vector<FieldBase> derivs;
    NodeList* nodeList;
public:
    State(NodeList* nodeList) : 
        nodeList(nodeList) {};

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


};

#endif //STATE_HH