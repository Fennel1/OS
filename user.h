#include "iNode.h"
#include "directory.h"

#ifndef USER_H
#define USER_H

class User{
private:
    Directory* cur_dir = nullptr;
    std::string username = "";
    std::string password = "";
    bool state = false;
public:
    User();
};

class Users{
private:
    int userListSize = 0;

public:
    std::vector<User> userList;
};

#endif //USER_H