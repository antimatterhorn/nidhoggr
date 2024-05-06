#ifndef STATE_HH
#define STATE_HH

#include "../DataBase/nodeList.hh"

class State {
private:
    FieldBase* field;
    NodeList* nodeList;
public:
    State(FieldBase* field, NodeList* nodeList) : 
        field(field),
        nodeList(nodeList) {};

    ~State() {};

    template <typename T>
    void
    copyField() {
        int numNodes = nodeList->size();
        std::string name = field->getNameString();
        if (nodeList->getField<T>("copy-of-"+name) == nullptr)
            nodeList->insertField<T>("copy-of-"+name);
        for (int i=0; i<numNodes; ++i)
            nodeList->getField<T>("copy-of-"+name)->setValue(i,field[i]);
    }

};

#endif //STATE_HH