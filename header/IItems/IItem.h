#ifndef IITEM_H
#define IITEM_H

#include <iostream>
#include <stdlib.h>

class IItem
{
    public:
        ~IItem() {};
        virtual std::string toString() const = 0;
};

#endif