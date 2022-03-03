#ifndef _PERSON_H_
#define _PERSON_H_

#include "Highlight.h"
#include <string>

using namespace std;

class Person{
    public:
        Person(const string &name, const int &id);
	    string getName() const;
        string getInfo() const;
        int getRegNo() const;
	    void changeName(const string &newName);

    protected:
	   string name;
       int regNo;
};

#endif
