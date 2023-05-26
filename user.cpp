#include "user.h"

User::User(std::string username, std::string password, Directory* cur_dir) {
    username_ = username;
    password_ = password;
    cur_dir_ = cur_dir;
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

Directory* User::getCurDir() {
    return cur_dir_;
}

void User::setCurDir(Directory* cur_dir) {
    cur_dir_ = cur_dir;
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
    cur_dir_ = nullptr;
    state_ = false;
}

void Users::readUserList() {

}

bool Users::isExist(std::string username) {
    for (int i = 0; i < userListSize; i++){
        if (userList_[i].getUserName() == username){
            return true;
        }
    }
    return false;
}

void Users::createUser(std::string username, std::string password) {
    if (!isExist(username)){
        userList_.push_back(User(username, password));
        userListSize++;
    }
}

void Users::saveUserList() {

}

bool Users::login(std::string username, std::string password) {
    for (int i = 0; i < userListSize; i++){
        if (userList_[i].login(username, password)){
            curr_user_ = username;
            return true;
        }
    }
    return false;
}

int Users::findUser(std::string username) {
    for (int i = 0; i < userListSize; i++){
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

Directory* Users::getCurDir() {
    int i = findUser(curr_user_);
    if (i == -1){
        return nullptr;
    }
    return userList_[i].getCurDir();
}

void Users::setCurDir(Directory* cur_dir) {
    int i = findUser(curr_user_);
    if (i == -1){
        return;
    }
    userList_[i].setCurDir(cur_dir);
}

bool Users::check() {
    int i = findUser(curr_user_);
    if (i == -1){
        return false;
    }
    return userList_[i].check();
}