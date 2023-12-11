#ifndef IRESPONSE_H
#define IRESPONSE_H

#include <string>
#include <iostream>
#include "IItem.h"

class IResponse
{
    protected:
        int socketId;
        IItem payload;

    public:
        IItem GetPayload();
};

#endif