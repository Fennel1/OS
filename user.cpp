#include "user.h"

User::User(std::string username, std::string password, int inode_id = -1) {
    username_ = username;
    password_ = password;
    inode_id_ = inode_id;
}

std::string User::getUserName() {
    return username_;
}

std::string User::getPassword() {
    return password_;
}

bool User::login(std::string username, std::string password) {
    if (username_ == username && password_ == password){
        state_ = true;
        return true;
    }
    return false;
}

int User::getInodeId() {
    return inode_id_;
}

void User::setInodeId(int inode_id) {
    inode_id_ = inode_id;
}

bool User::check() {
    return state_;
}

void User::logOut() {
    state_ = false;
}

void User::clear() {
    username_ = "";
    password_ = "";
    inode_id_ = -1;
    state_ = false;
}

void Users::readUserList() {

}

bool Users::isExist(std::string username) {
    for (int i = 0; i < userList_.size(); i++){
        if (userList_[i].getUserName() == username){
            return true;
        }
    }
    return false;
}

void Users::createUser(std::string username, std::string password) {
    if (!isExist(username)){
        userList_.push_back(User(username, password));
    }
}

void Users::saveUserList() {

}

bool Users::login(std::string username, std::string password) {
    for (int i = 0; i < userList_.size(); i++){
        if (userList_[i].getUserName() == username){
            if (!userList_[i].check()){
                if (userList_[i].login(username, password)){
                    curr_user_ = username;
                    return true;
                }
            }
            break;
        }
    }
    return false;
}

int Users::findUser(std::string username) {
    for (int i = 0; i < userList_.size(); i++){
        if (userList_[i].getUserName() == username){
            return i;
        }
    }
    return -1;
}

bool Users::switchUser(std::string username) {
    int id = findUser(username);
    if (id == -1){
        return false;
    }
    if (userList_[id].check()){
        curr_user_ = username;
        return true;
    }
    return false;
}

void Users::logOut() {
    int i = findUser(curr_user_);
    if (i == -1){
        return;
    }
    userList_[i].logOut();
    curr_user_ = "";
}

int Users::getInodeId() {
    int i = findUser(curr_user_);
    if (i == -1){
        return -1;
    }
    return userList_[i].getInodeId();
}

void Users::setInodeId(int inode_id) {
    int i = findUser(curr_user_);
    if (i == -1){
        return;
    }
    userList_[i].setInodeId(inode_id);
}

bool Users::check() {
    int i = findUser(curr_user_);
    if (i == -1){
        return false;
    }
    return userList_[i].check();
}