/* 
I'm including a name class to eventually protect this against inserting code into the name field
used throughout the code, and to implement simple bool checking with case-insensitivity. 
*/

#ifndef NAME_HH
#define NAME_HH

#include <string>

class Name {
private:
    std::string myName;

public:
    Name() {}

    Name(const std::string& name) : myName(name) {}

    std::string name() const { return myName; }

    bool empty() const { return myName.empty(); }
};

#endif // NAME_HH