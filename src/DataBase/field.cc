#ifndef FIELD_CC
#define FIELD_CC

#include "field.hh"

template <typename T>
Field<T>::Field() {}

template <typename T>
Field<T>::Field(const std::string& fieldName)
    : name(fieldName) {}

template <typename T>
Field<T>::Field(const std::string& fieldName, unsigned int numElements)
    : name(fieldName) {
    this->fill(numElements, T());
}

template <typename T>
void Field<T>::addValue(const T& value) {
    values.push_back(value);
}

template <typename T>
unsigned int Field<T>::getSize() const { 
    return values.size(); 
}

template <typename T>
unsigned int Field<T>::size() const { 
    return values.size(); 
}

template <typename T>
const std::vector<T>& Field<T>::getValues() const { 
    return values; 
}

template <typename T>
const T& Field<T>::getValue(const unsigned int index) const { 
    return values[index]; 
}

template <typename T>
void Field<T>::setValue(const unsigned int index, T val) { 
    values[index] = val; 
}

template <typename T>
T& Field<T>::operator[](const unsigned int index) { 
    return values[index]; 
}

template <typename T>
const T& Field<T>::operator[](const unsigned int index) const { 
    return values[index]; 
}

template <typename T>
void Field<T>::copyValues(const Field<T>& other) {
    if (this != &other) 
        values = other.values;
}

template <typename T>
void Field<T>::copyValues(const Field<T>* other) {
    values = other->values;
}

template <typename T>
Field<T>& Field<T>::operator=(const Field<T>& other) {
    if (this != &other) { // Avoid self-assignment
        values = other.values;
        name = other.name;
    }
    return *this;
}

template <typename T>
Field<T> Field<T>::operator+(const Field<T>& other) const {
    Field<T> result(*this); // Create a copy of the current object
    for (int i = 0; i < this->size(); ++i) {
        result.setValue(i, this->getValue(i) + other.getValue(i)); // Perform element-wise addition
    }
    return result; // Return the result
}

template <typename T>
Field<T> Field<T>::operator-(const Field<T>& other) const {
    Field<T> result(*this); // Create a copy of the current object
    for (int i = 0; i < this->size(); ++i) {
        result.setValue(i, this->getValue(i) - other.getValue(i)); // Perform element-wise addition
    }
    return result; // Return the result
}

template <typename T>
Field<T> Field<T>::operator*(const double other) const {
    Field<T> result(*this); // Create a copy of the current object
    for (int i = 0; i < this->size(); ++i) {
        result.setValue(i, this->getValue(i) * other); // Perform element-wise scalar multiplication
    }
    return result; // Return the result
}

template <typename T>
Field<T>& Field<T>::operator+=(const Field<T>& other) {
    if (this != &other) {
        for (int i = 0; i < this->size(); ++i) {
            this->setValue(i, this->getValue(i) + other.getValue(i)); // Perform element-wise addition
        }
    }
    return *this;
}

template <typename T>
Field<T>& Field<T>::operator*=(const double other) {
    for (int i = 0; i < this->size(); ++i) {
        this->setValue(i, this->getValue(i) * other); // Perform element-wise scalar multiplication
    }
    return *this;
}

template <typename T>
bool Field<T>::hasName() const {
    return !name.name().empty();
}

template <typename T>
Name Field<T>::getName() const {
    return name;
}

template <typename T>
std::string Field<T>::getNameString() const {
    return name.name();
}

template <typename T>
void Field<T>::fill(unsigned int n, T val) {
    for (unsigned int i = 0; i < n; ++i) {
        this->addValue(val);
    }
}

#endif // FIELD_CC
