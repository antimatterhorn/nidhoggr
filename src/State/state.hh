#ifndef STATE_HH
#define STATE_HH

#include "../EOS/equationOfStateBase.hh"
#include "../DataBase/nodeList.hh"

class State {
private:
    EquationOfState* _eos;
    NodeList* _nodeList;
public:
    State(NodeList* nodeList, EquationOfState* eos) : 
        _nodeList(nodeList), 
        _eos(eos) {};

    ~State() {};

    NodeList* 
    nodeList() { return _nodeList; }
    
    EquationOfState* 
    eos() { return _eos; }
};

#endif //STATE_HH