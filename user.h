#include "iNode.h"
#include "directory.h"

#ifndef USER_H
#define USER_H

class User{
private:
    Directory* cur_dir_ = nullptr;
    std::string username_ = "";
    std::string password_ = "";
    bool state_ = false;
public:
    User() = default;
    User(std::string username, std::string password, Directory* cur_dir=nullptr);

    std::string getUserName();
    std::string getPassword();
    bool login(std::string username, std::string password);

    Directory* getCurDir();
    void setCurDir(Directory* cur_dir);
    bool check();
    void logOut();
    void clear();
};

class Users{
private:
    int userListSize = 0;
    std::string curr_user_ = "";

public:
    std::vector<User> userList_;

    Users() = default;
    void readUserList();
    bool isExist(std::string username);
    void createUser(std::string username, std::string password);
    void saveUserList();
    bool login(std::string username, std::string password);
    int findUser(std::string username);
    bool switchUser(std::string username);
    void logOut();
    Directory* getCurDir();
    void setCurDir(Directory* cur_dir);
    bool check();
};

#endif //USER_H