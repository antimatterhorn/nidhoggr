#ifndef FIELD_HH
#define FIELD_HH

#include <string>
#include <vector>
#include "../Type/name.hh"

// Base class for all Field types
class FieldBase {
public:
    virtual ~FieldBase() {}  // Make the base class polymorphic

    virtual bool hasName() const = 0;
    virtual Name getName() const = 0;
    virtual std::string getNameString() const = 0;
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

    size_t getSize() const { return values.size(); }

    size_t size() const { return getSize(); }

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

    Field<T> operator+(const Field<T>& other) const {
        return add(other);
    }

    Field<T> operator-(const Field<T>& other) const {
        return sub(other);
    }

    Field<T> operator*(const double other) const {
        return scalarProduct(other);
    }
    
    Field<T> add(const Field<T>& other) const {
        Field<T> result;
        for (int i = 0; i < this->size(); ++i) {
            result.values[i] = values[i] + other.values[i];
        }
        result.name = this->name;
        return result;
    }

    Field<T> sub(const Field<T>& other) const {
        Field<T> result;
        for (int i = 0; i < this->size(); ++i) {
            result.values[i] = values[i] - other.values[i];
        }
        result.name = this->name;
        return result;
    }

    Field<T> scalarProduct(const double other) const {
        Field<T> result;
        for (int i = 0; i < this->size(); ++i) {
            result.values[i] = values[i] * other;
        }
        result.name = this->name;
        return result;
    }

    bool hasName() const override {
        return !name.empty();
    }

    Name getName() const override {
        return name;
    }

    std::string getNameString() const override {
        return name.name();
    }
};

#endif // FIELD_HH
