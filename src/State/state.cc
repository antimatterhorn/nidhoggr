#include <vector>
#include "../Field/field.cc" 

class State {
private:
    std::vector<FieldListBase*> fieldLists;
public:
    State() {}

    template<typename T>
    void addFieldList(const FieldList<T>& fieldList) {
        fieldLists.push_back(fieldList);
    }

    void addState(const State& other) {
        fieldLists.insert(fieldLists.end(), other.fieldLists.begin(), other.fieldLists.end());
    }

    State operator+(const State& other) const {
        State newState = *this; 
        newState.addState(other);
        return newState;
    }

    State& operator+=(const State& other) {
        addState(other);
        return *this;
    }

    std::vector<FieldListBase*> getFieldLists() { return fieldLists; }

    template<typename T>
    FieldList<T>& getFieldList(size_t index) {
        return fieldLists[index];
    }

    size_t getFieldListCount() const {
        return fieldLists.size();
    }

    ~State() {
        for (auto fieldList : fieldLists) {
            delete fieldList;
        }
    }
};
