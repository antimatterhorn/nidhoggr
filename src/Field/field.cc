#include <string>
#include <vector>
#include <typeinfo>

template <typename T>
class Field {
private:
    T value;
    std::string name;
public:
    Field() {}

    Field(const std::string& fieldName)
        : name(fieldName) {}

    Field(const std::string& fieldName, const T& fieldValue)
        : value(fieldValue), name(fieldName) {}
    
    ~Field() {}

    Field& operator=(const T& newValue) {
        value = newValue;
        return *this;
    }

    T getValue() const { return value; }
    std::string getName() const { return name; }
    std::string getType() const { return typeid(value).name(); }
};

template <typename T>
class FieldList {
private:
    std::vector<Field<T>> fields;

public:
    FieldList() {}

    FieldList(const Field<T>& field, size_t numFields) {
        for (size_t i = 0; i < numFields; ++i) {
            fields.push_back(field);
        }
    }

    void addField(const Field<T>& field) {
        fields.push_back(field);
    }

    size_t getSize() const {
        return fields.size();
    }

    Field<T>& getField(size_t index) {
        return fields[index];
    }

    Field<T>& operator[](size_t index) {
        return fields[index];
    }
};