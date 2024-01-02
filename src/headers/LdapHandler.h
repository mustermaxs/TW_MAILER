#ifndef LDAPHANDLER_H
#define LDAPHANDLER_H

#include <ldap.h>
#include <string>


class LdapHandler
{
    public:
        LdapHandler();
        ~LdapHandler();
        bool tryLoginUser(std::string, std::string);
};

#endif
