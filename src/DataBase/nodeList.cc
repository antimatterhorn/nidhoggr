#ifndef NODELIST_CC
#define NODELIST_CC

#include "nodeList.hh"

NodeList::NodeList() {}

NodeList::NodeList(int numNodes) {  
    _ids = Field<int>("id");     
    for (int i = 0; i < numNodes; ++i) { 
        _ids.addValue(i); 
    }
    addField(std::make_shared<Field<int>>(_ids));
}

NodeList::~NodeList() {}

void NodeList::addField(const std::shared_ptr<FieldBase>& fieldPtr) {
    _fields.push_back(fieldPtr);
}

void NodeList::addNodeList(const NodeList& other) {
    _fields.insert(_fields.end(), other._fields.begin(), other._fields.end());
}

NodeList NodeList::operator+(const NodeList& other) const {
    NodeList newNodeList = *this; 
    newNodeList.addNodeList(other);
    return newNodeList;
}

NodeList& NodeList::operator+=(const NodeList& other) {
    addNodeList(other);
    return *this;
}

Field<double>* NodeList::mass() const {
    return getFieldByName<double>(Name("mass"));
}

size_t NodeList::getFieldCount() const {
    return _fields.size();
}

size_t NodeList::getNumNodes() const {
    return _fields[0]->getSize();
}

std::vector<std::string> NodeList::fieldNames() const {
    std::vector<std::string> names;
    for (const auto& field : _fields) {
        if (field && field->hasName()) {
            names.push_back(field->getNameString());
        }
    }
    return names;
}

Field<int>& NodeList::nodes() {
    return _ids;
}

unsigned int NodeList::size() const {
    return getNumNodes();
}

// Template Implementations
template <typename T>
void NodeList::insertField(const std::string& name) {
    auto newField = std::make_shared<Field<T>>(name, this->size());
    _extraFields.push_back(newField);
    addField(newField);
}

template <typename T>
Field<T>* NodeList::getFieldByName(const Name& name) const {
    for (const auto& fieldPtr : _fields) {
        if (fieldPtr->hasName() && fieldPtr->getNameString() == name.name()) {
            Field<T>* castedField = dynamic_cast<Field<T>*>(fieldPtr.get());
            if (castedField != nullptr) {
                return castedField;
            }
        }
    }
    return nullptr;
}

template <typename T>
Field<T>* NodeList::getField(const std::string& name) const {
    return getFieldByName<T>(Name(name));
}

template <int dim>
Field<Lin::Vector<dim>>* NodeList::velocity() const {
    for (const auto& fieldPtr : _fields) {
        if (fieldPtr->hasName() && fieldPtr->getName() == Name("velocity")) {
            Field<Lin::Vector<dim>>* velocityField = dynamic_cast<Field<Lin::Vector<dim>>*>(fieldPtr.get());
            if (velocityField != nullptr) {
                return velocityField;
            }
        }
    }
    return nullptr;
}

template <int dim>
Field<Lin::Vector<dim>>* NodeList::position() const {
    for (const auto& fieldPtr : _fields) {
        if (fieldPtr->hasName() && fieldPtr->getName() == Name("position")) {
            Field<Lin::Vector<dim>>* positionField = dynamic_cast<Field<Lin::Vector<dim>>*>(fieldPtr.get());
            if (positionField != nullptr) {
                return positionField;
            }
        }
    }
    return nullptr;
}


#endif