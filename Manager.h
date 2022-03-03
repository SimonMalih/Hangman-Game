#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Person.h"
#include <string>

class Manager: public Person{
public:
    Manager(int regNo, const string &name);
};
#endif
