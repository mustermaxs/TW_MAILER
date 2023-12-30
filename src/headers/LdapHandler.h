#ifndef LDAPHANDLER_H
#define LDAPHANDLER_H

#include <string>


class LdapHandler
{
    public:
        LdapHandler();
        ~LdapHandler();
        bool tryLoginUser(std::string, std::string);
};

#endif
