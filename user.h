#include "iNode.h"
#include "directory.h"

#ifndef USER_H
#define USER_H

class User{
private:
    int inode_id_ = -1; // inode号
    std::string username_ = "";
    std::string password_ = "";
    bool state_ = false; // 0: logout, 1: login
public:
    User() = default;
    User(std::string username, std::string password, int inode_id = -1);

    std::string getUserName();
    std::string getPassword();
    bool login(std::string username, std::string password);

    int getInodeId();
    void setInodeId(int inode_id);
    bool check();
    void logOut();
    void clear();
};

class Users{
private:

public:
    std::string curr_user_ = "";
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
    int getInodeId();
    void setInodeId(int inode_id);
    bool check();
};

#endif //USER_H