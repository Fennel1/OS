#include "ram.h"


FileOpenItem::FileOpenItem(unsigned offSet, int sign, int mode, int inode_id) {
    this->offSet = offSet;
    this->sign = sign;
    this->mode = mode;
    this->inode_id = inode_id;
    linkN = 1;
}

FileOpenItem::FileOpenItem(const FileOpenItem& thx) {
    offSet = thx.offSet;
    sign = thx.sign;
    mode = thx.mode;
    inode_id = thx.inode_id;
    linkN = thx.linkN;
}

int FileOpenItem::getINodeId() {
    return inode_id;
}

int FileOpenItem::getOffSet() {
    return offSet;
}

bool FileOpenItem::setOffSet(unsigned offSet) {
    this->offSet = offSet;
    return true;
}

void FileOpenItem::clear() {
    offSet = 0;
    sign = 0;
    mode = 0;
    inode_id = -1;
    linkN = 0;
}

int FileOpenItem::getLinkN() {
    return linkN;
}

void FileOpenItem::addLink() {
    ++linkN;
}

bool FileOpenItem::deleteLink() {
    --linkN;
    return linkN == 0;
}

int FileOpenItem::getMode() {
    return mode;
}

int FileOpenItem::getSign() {
    return sign;
}

int FileOpenItem::setSign(int sign) {
    this->sign = sign;
    return sign;
}

void FileOpenList::clear() {
    for (int i = 0; i < capacity_; ++i) {
        fileOpenList_[i].clear();
    }
    size_ = 0;
}

int FileOpenList::getSize() {
    return size_;
}

int FileOpenList::getOffSet(int id) {
    return fileOpenList_[id].getOffSet();
}

void FileOpenList::setOffSet(int id, unsigned offSet) {
    fileOpenList_[id].setOffSet(offSet);
}

int FileOpenList::getMode(int id) {
    return fileOpenList_[id].getMode();
}

int FileOpenList::getSign(int id) {
    return fileOpenList_[id].getSign();
}

int FileOpenList::setSign(int id, int sign) {
    return fileOpenList_[id].setSign(sign);
}

bool FileOpenList::deleteItem(int id) {
    if (fileOpenList_[id].getLinkN() < 0){
       return false;
    }
    fileOpenList_[id].clear();
    --size_;
    return true;
}

int FileOpenList::addItem(unsigned offset, int sign, int mode, int inode_id)
{
    int i = getFreeItem();
    if(i == -1)
        return -1;
    fileOpenList_[i] = FileOpenItem(offset, sign, mode, inode_id);
    size_++;
    return i; // 返回系统文件打开表的下标
}

int FileOpenList::getFreeItem() {
    for (int i = 0; i < capacity_; ++i) {
        if (fileOpenList_[i].getLinkN() == 0){
            return i;
        }
    }
    return -1;
}

int FileOpenList::getItemInodeId(int id) {
    if (fileOpenList_[id].getLinkN() == 0){
        return -1;
    }
    else{
        return fileOpenList_[id].getINodeId();
    }
}

bool FileOpenList::addLink(int id) {
    if (fileOpenList_[id].getLinkN() < 0){
        return false;
    }
    fileOpenList_[id].addLink();
    return true;
}

bool FileOpenList::deleteLink(int id) {
    if (fileOpenList_[id].getLinkN() <= 0){
        return false;
    }
    fileOpenList_[id].deleteLink();
    // if (fileOpenList_[id].getLinkN() == 0){
    //     deleteItem(id);
    //     return true;
    // }
    return true;
}

int FileOpenList::getLinkN(int id) {
    return fileOpenList_[id].getLinkN();
}

UserOpenItem::UserOpenItem(int inode_id, int id) {
    this->inode_id = inode_id;
    this->id = id;
}

void UserOpenItem::clear() {
    inode_id = -1;
    id = -1;
}

int UserOpenItem::getINodeId() {
    return inode_id;
}

int UserOpenItem::getId() {
    return id;
}

void UserOpenItem::set(int inode_id, int id) {
    this->inode_id = inode_id;
    this->id = id;
}

UserOpenList::UserOpenList(std::string username) {
    username_ = username;
}

void UserOpenList::clear() {
    username_ = "";
    for (int i = 0; i < MAX_USER_FD; ++i) {
        open_list_[i].clear();
    }
}

int UserOpenList::getFileId(int iNodeid) {
    int id = findId(iNodeid);
    if (id == -1){
        return -1;
    }
    return open_list_[id].getId();
}

bool UserOpenList::addItem(int iNodeId, int fileId){
    int id = findFreeItemId();
    if (id == -1){
        return false;
    }
    open_list_[id].set(iNodeId, fileId);
    return true;
}

bool UserOpenList::deleteItem(int iNodeId){
    int id = findId(iNodeId);
    if (id == -1){
        return false;
    }
    open_list_[id].clear();
    return true;
}

int UserOpenList::findId(int iNodeId) {
    for (int i = 0; i < MAX_USER_FD; ++i) {
        if (open_list_[i].getINodeId() == iNodeId){
            return i;
        }
    }
    return -1;
}

int UserOpenList::findFreeItemId() {
    for (int i = 0; i < MAX_USER_FD; ++i) {
        if (open_list_[i].getId() == -1){
            return i;
        }
    }
    return -1;
}

std::string UserOpenList::getUsername() {
    return username_;
}