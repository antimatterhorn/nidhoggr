#ifndef NAME_CC
#define NAME_CC

#include "name.hh"

bool Name::isValidName(const std::string& name) const {
    for (char c : name) {
        // Check if the character is not alphanumeric
        if (!std::isalnum(c)) {
            return false;
        }
    }
    return true;
}

Name::Name() {
    myName = "noName";
}

Name::Name(const std::string& name) {
    // Validate the input string
    if (!isValidName(name)) {
        std::cerr << "Invalid name provided: " << name << std::endl;
        throw std::invalid_argument("Invalid name provided");
    }
    myName = name;
}

std::string Name::name() const {
    if (myName.size() > 0)
        return myName;
    else
        return "noName";
}

bool Name::empty() const {
    return myName.empty();
}

bool Name::operator==(const Name& other) const {
    return compare(other.name());
}

bool Name::operator==(const std::string& other) const {
    return compare(other);
}

bool Name::compare(const std::string other) const {
    std::string name1_lower = myName;
    std::string name2_lower = other;
    std::transform(name1_lower.begin(), name1_lower.end(), name1_lower.begin(), ::tolower);
    std::transform(name2_lower.begin(), name2_lower.end(), name2_lower.begin(), ::tolower);

    // Compare the lowercase strings for equality
    return name1_lower == name2_lower;
}

Name& Name::operator=(const std::string& name) {
    if (!isValidName(name)) {
        throw std::invalid_argument("Invalid name provided");
    }
    myName = name;
    return *this;
}

#endif