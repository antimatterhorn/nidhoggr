#ifndef FIELD_HH
#define FIELD_HH

#include <string>
#include <vector>
#include "name.hh"

// Base class for all Field types
class FieldBase {
public:
    virtual ~FieldBase() {}  // Make the base class polymorphic

    virtual bool hasName() const = 0;
    virtual std::string getName() const = 0;
    virtual size_t getSize() const = 0;
};

template <typename T>
class Field : public FieldBase {
private:
    std::vector<T> values;
    Name name;

public:
    Field() {}

    Field(const std::string& fieldName)
        : name(fieldName) {}

    void addValue(const T& value) {
        values.push_back(value);
    }

    size_t getSize() const {
        return values.size();
    }

    const std::vector<T>& getValues() const {
        return values;
    }

    T& getValue(size_t index) {
        return values[index];
    }

    T& operator[](size_t index) {
        return values[index];
    }

    Field<T>& operator=(const Field<T>& other) {
        if (this != &other) { // Avoid self-assignment
            values = other.values;
            name = other.name;
        }
        return *this;
    }

    Field<T>& operator+(const T& other) {
        addValue(other);
        return *this;
    }

    bool hasName() const override {
        return !name.empty();
    }

    std::string getName() const override {
        return name.name();
    }
};

#endif // FIELD_HH
