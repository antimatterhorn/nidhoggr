#ifndef FIELD_HH
#define FIELD_HH

#include <string>
#include <vector>
#include <memory>
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
    Field();
    Field(const std::string& fieldName);
    Field(const std::string& fieldName, unsigned int numElements);

    void addValue(const T& value);
    unsigned int getSize() const override;
    unsigned int size() const override;
    const std::vector<T>& getValues() const;
    const T& getValue(const unsigned int index) const;
    void setValue(const unsigned int index, T val);
    T& operator[](const unsigned int index);
    const T& operator[](const unsigned int index) const;

    void copyValues(const Field<T>& other);
    void copyValues(const Field<T>* other);

    Field<T>& operator=(const Field<T>& other);
    Field<T> operator+(const Field<T>& other) const;
    Field<T> operator-(const Field<T>& other) const;
    Field<T> operator*(const double other) const;
    Field<T>& operator+=(const Field<T>& other);
    Field<T>& operator-=(const Field<T>& other);
    Field<T>& operator*=(const double other);

    bool hasName() const override;
    Name getName() const override;
    std::string getNameString() const override;

    void fill(unsigned int n, T val);
};

#include "field.cc"

#endif // FIELD_HH
