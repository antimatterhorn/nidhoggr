/* 
I'm including a name class to eventually protect this against inserting code into the name field
used throughout the code, and to implement simple bool checking with case-insensitivity. 
*/

#ifndef NAME_HH
#define NAME_HH

#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>

class Name {
private:
    std::string myName;

    bool isValidName(const std::string& name) const {
        for (char c : name) {
            // Check if the character is not alphanumeric
            if (!std::isalnum(c)) {
                return false;
            }
        }
        return true;
    }
public:
    Name() {}

    Name(const std::string& name) {
        // Validate the input string
        if (!isValidName(name)) {
            throw std::invalid_argument("Invalid name provided");
        }
        myName = name;
    }

    std::string name() const { return myName; }

    bool empty() const { return myName.empty(); }

    bool operator==(const Name& other) const {
        return compare(other.name());
    }

    bool operator==(const std::string& other) const {
        return compare(other);
    }

    bool compare(const std::string other) const {
        std::string name1_lower = myName;
        std::string name2_lower = other;
        std::transform(name1_lower.begin(), name1_lower.end(), name1_lower.begin(), ::tolower);
        std::transform(name2_lower.begin(), name2_lower.end(), name2_lower.begin(), ::tolower);

        // Compare the lowercase strings for equality
        return name1_lower == name2_lower;
    }

    Name& operator=(const std::string& name) {
        if (!isValidName(name)) {
            throw std::invalid_argument("Invalid name provided");
        }
        myName = name;
        return *this;
    }
};

#endif // NAME_HH