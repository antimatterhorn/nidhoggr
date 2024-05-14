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

    const T& 
    getValue(const unsigned int index) const { return values[index]; }

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

    Field<T> operator+(const Field<T>& other) const {
        Field<T> result(*this); // Create a copy of the current object
        for (int i = 0; i < this->size(); ++i) {
            result.setValue(i, this->getValue(i) + other.getValue(i)); // Perform element-wise addition
        }
        return result; // Return the result
    }

    Field<T> operator-(const Field<T>& other) const {
        Field<T> result(*this); // Create a copy of the current object
        for (int i = 0; i < this->size(); ++i) {
            result.setValue(i, this->getValue(i) - other.getValue(i)); // Perform element-wise addition
        }
        return result; // Return the result
    }

    Field<T> operator*(const double other) const {
        Field<T> result(*this); // Create a copy of the current object
        for (int i = 0; i < this->size(); ++i) {
            result.setValue(i, this->getValue(i) * other); // Perform element-wise scalar multiplication
        }
        return result; // Return the result
    }

    Field<T>& operator+=(const Field<T>& other) {
        if (this != &other) {
            for (int i = 0; i < this->size(); ++i) {
                this->setValue(i, this->getValue(i) + other.getValue(i)); // Perform element-wise addition
            }
        }
        return *this;
    }

    Field<T>& operator*=(const double other) {
        for (int i = 0; i < this->size(); ++i) {
            this->setValue(i, this->getValue(i) * other); // Perform element-wise scalar multiplication
        }
        return *this;
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