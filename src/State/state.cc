#include "../EOS/equationOfStateBase.hh"
#include "../DataBase/nodeList.hh"

class State {
private:
    EquationOfState* eosPtr;
    NodeList* nodeListPtr;
public:
    State(NodeList* nodeList, EquationOfState* eos) : nodeListPtr(nodeList), eosPtr(eos) {};

    ~State() {};

    NodeList* getNodeList() { return nodeListPtr; }
    EquationOfState* getEOS() { return eosPtr; }
};