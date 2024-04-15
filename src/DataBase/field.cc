#include <string>
#include <vector>
#include <typeinfo>


// Base class for all FieldList types
class FieldListBase {
public:
    virtual ~FieldListBase() {}  // Make the base class polymorphic

    virtual bool hasName() const = 0; 
    virtual std::string getName() const = 0;
    virtual size_t getSize() const = 0;
};

template <typename T>
class FieldList : public FieldListBase {
private:
    std::vector<T> fields;
    std::string name;
public:
    FieldList() {}

    FieldList(const std::string& fieldName, size_t numFields) 
        : name(fieldName) { fields.resize(numFields); }

    void addField(const T& field) {
        fields.push_back(field);
    }

    size_t getSize() const {
        return fields.size();
    }

    const std::vector<T>& getFields() const { return fields; }

    T& getField(size_t index) { return fields[index]; }

    T& operator[](size_t index) {
        return fields[index];
    }

    FieldList<T>& operator=(const FieldList<T>& other) {
        if (this != &other) { // Avoid self-assignment
            fields = other.fields;
        }
        return *this;
    }

    FieldList<T>& operator+(const T& other) {
        addField(other);
        return *this;
    }

    // Method to check if the FieldList has a name
    bool hasName() const override { return !name.empty(); }

    std::string getName() const override { return name; }
};