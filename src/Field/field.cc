#include <string>

template <typename T>
class Field {
private:
    T value;
    std::string name;
public:
    Field(const std::string& fieldName, const T& fieldValue)
        : value(fieldValue), name(fieldName) {}
    
    ~Field() {}

    T getValue() const { return value; }
    std::string getName() const { return name; }
};