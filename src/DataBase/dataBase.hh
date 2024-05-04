#ifndef DATABASE_HH
#define DATABASE_HH

#include "../DataBase/nodeList.hh"

class DataBase {
private:
    
public:
    std::vector<NodeList*> nodeLists;
    
    DataBase() {}

    ~DataBase() {}

    void
    EnrollNodeList(NodeList* nodeList) {
        nodeLists.push_back(nodeList);
    }
};


#endif // DATABASE_HH