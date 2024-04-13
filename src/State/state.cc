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
