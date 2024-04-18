#ifndef FIELDLIST_HH
#define FIELDLIST_HH

#include <string>
#include <vector>

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
    std::vector<std::pair<std::string, T>> fields;

public:
    FieldList() {}

    FieldList(const std::string& fieldName, size_t numFields) {
        fields.reserve(numFields); // Reserve memory for efficiency
        for (size_t i = 0; i < numFields; ++i) {
            fields.emplace_back(fieldName, T()); // Initialize with default value of T
        }
    }

    void addField(const std::pair<std::string, T>& field) {
        fields.push_back(field);
    }

    size_t getSize() const {
        return fields.size();
    }

    const std::vector<std::pair<std::string, T>>& getFields() const {
        return fields;
    }

    T& getField(size_t index) { return fields[index].second; }

    T& operator[](size_t index) { return fields[index].second; }

    FieldList<T>& operator=(const FieldList<T>& other) {
        if (this != &other) { // Avoid self-assignment
            fields = other.fields;
        }
        return *this;
    }

    FieldList<T>& operator+(const std::pair<std::string, T>& other) {
        addField(other);
        return *this;
    }

    bool hasName() const override {
        return !fields.empty() && !fields[0].first.empty();
    }

    std::string getName() const override {
        return hasName() ? fields[0].first : "";
    }
};

#endif // FIELDLIST_HH