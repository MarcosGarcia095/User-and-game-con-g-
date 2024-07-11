#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    int id;
    std::string name;
    std::string email;

    User(int id, const std::string& name, const std::string& email) : id(id), name(name), email(email) {}
};

#endif
