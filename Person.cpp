#include "Person.h"

Person::Person(const string &name, const int &id){
    this->name = name;
    this->regNo = id;
}

string Person::getName() const{
    return name;
}

int Person::getRegNo() const {
    return regNo;
}

string Person::getInfo() const {
    return to_string(regNo)+"|"+name+"|";
}

void Person::changeName(const string &newName){
    name = newName;
}
