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
    virtual unsigned int getSize() const = 0;
    virtual unsigned int size() const = 0;
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
    
    Field(const std::string& fieldName, unsigned int numElements)
        : name(fieldName) {
        this->fill(numElements,T());
    }

    void 
    addValue(const T& value) {
        values.push_back(value);
    }

    unsigned int 
    getSize() const override { return values.size(); }

    unsigned int 
    size() const override { return values.size(); }

    const std::vector<T>& 
    getValues() const { return values; }

    T& 
    getValue(const unsigned int index) { return values[index]; }

    void
    setValue(const unsigned int index, T val) { values[index] = val; }

    T& 
    operator[](const unsigned int index) { return values[index]; }

    const T& 
    operator[](const unsigned int index) const { return values[index]; }

    void
    copyValues(const Field<T>& other) {
        if (this!= &other) 
            values = other.values;
    }
    void
    copyValues(const Field<T>* other) {
        values = other->values;
    }

    Field<T>& 
    operator=(const Field<T>& other) {
        if (this != &other) { // Avoid self-assignment
            values = other.values;
            name = other.name;
        }
        return *this;
    }

    Field<T> 
    operator+(const Field<T>& other) const {
        return add(other);
    }

    Field<T> 
    operator-(const Field<T>& other) const {
        return sub(other);
    }

    Field<T> 
    operator*(const double other) const {
        return scalarProduct(other);
    }
    
    Field<T> 
    add(const Field<T>& other) const {
        Field<T> result;
        for (int i = 0; i < this->size(); ++i) {
            result.values[i] = values[i] + other.values[i];
        }
        result.name = this->name;
        return result;
    }

    Field<T> 
    sub(const Field<T>& other) const {
        Field<T> result;
        for (int i = 0; i < this->size(); ++i) {
            result.values[i] = values[i] - other.values[i];
        }
        result.name = this->name;
        return result;
    }

    Field<T> 
    scalarProduct(const double other) const {
        Field<T> result;
        for (int i = 0; i < this->size(); ++i) {
            result.values[i] = values[i] * other;
        }
        result.name = this->name;
        return result;
    }

    bool 
    hasName() const override {
        if(name.name().size() > 0)
            return true;
        else
            return false;
    }

    Name 
    getName() const override {
        return name;
    }

    std::string 
    getNameString() const override {
        return name.name();
    }

    void
    fill(unsigned int n, T val) {
        for(int i=0;i<n;i++)
        {
            this->addValue(val);
        }
    }

};

#endif // FIELD_HH