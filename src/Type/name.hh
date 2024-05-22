#ifndef NAME_HH
#define NAME_HH

#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <iostream>

class Name {
private:
    std::string myName;

    bool isValidName(const std::string& name) const;

public:
    Name();
    Name(const std::string& name);

    std::string name() const;
    bool empty() const;
    bool operator==(const Name& other) const;
    bool operator==(const std::string& other) const;
    bool compare(const std::string other) const;
    Name& operator=(const std::string& name);
};

#include "name.cc"

#endif // NAME_HH
