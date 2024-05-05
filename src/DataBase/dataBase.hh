#ifndef DATABASE_HH
#define DATABASE_HH

#include "../DataBase/nodeList.hh"
#include "../State/state.hh"

class DataBase {
private:
    
public:
    std::vector<NodeList*> nodeLists;
    std::vector<State*> states;

    DataBase() {}

    ~DataBase() {}

    void
    EnrollNodeList(NodeList* nodeList, State* state) {
        nodeLists.push_back(nodeList);
        states.push_back(state);
    }
};


#endif // DATABASE_HH