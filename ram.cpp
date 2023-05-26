#include "ram.h"

INodeListInRam::INodeListInRam() {
    for (int i = 0; i < capacity_; ++i) {
        iNodeNum_[i] = -1;
        fileLock_[i] = 0;
    }
}

int INodeListInRam::getFreeINode() {
    for (int i = 0; i < capacity_; ++i) {
        if (iNodeList_[i].getLink() == 0){
            return i;
        }
    }
    return -1;
}

bool INodeListInRam::loadINode(int id, INode iNode) {
    int i = getFreeINode();
    if (i == -1){
        return false;
    }
    iNodeList_[i] = iNode;
    iNodeNum_[i] = id;
    ++size_;
    return true;
}

INode INodeListInRam::freeINode(int id) {
    int i = findINode(id);
    if (i == -1){
        return INode();
    }
    INode iNode = iNodeList_[i];
    iNodeList_[i].clear();
    iNodeNum_[i] = -1;
    fileLock_[i] = 0;
    --size_;
    return iNode;
}

INode& INodeListInRam::getINode(int id) {
    int i = findINode(id);
    return iNodeList_[i];
}

int INodeListInRam::findINode(int id) {
    for (int i = 0; i < capacity_; ++i) {
        if (iNodeNum_[i] == id){
            return i;
        }
    }
    return -1;
}

bool INodeListInRam::setLock(std::string username, int lock, int id) {
    if (lock < 0 || lock > 2){
        return false;
    }
    int i = findINode(id);
    if (iNodeList_[i].getUserName() == username){
        fileLock_[i] = lock;
        return true;
    }
    return false;
}

FileOpenItem::FileOpenItem(unsigned offSet, int flags, int mode, int id) {
    this->offSet = offSet;
    this->flags = flags;
    this->mode = mode;
    this->id = id;
    linkN = 1;
}

FileOpenItem::FileOpenItem(const FileOpenItem& thx) {
    offSet = thx.offSet;
    flags = thx.flags;
    mode = thx.mode;
    id = thx.id;
    linkN = thx.linkN;
}

int FileOpenItem::getId() {
    return id;
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
    flags = 0;
    mode = 0;
    id = -1;
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

void FileOpenList::clear() {
    for (int i = 0; i < capacity_; ++i) {
        if (fileOpenList_[i].getLinkN() != 0){
            fileOpenList_[i].clear();
        }
    }
    fileOpenSize_ = 0;
}

int FileOpenList::getSize() {
    return fileOpenSize_;
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

bool FileOpenList::deleteItem(int id) {
    if (fileOpenList_[id].getLinkN() < 0){
       return false;
    }
    fileOpenList_[id].clear();
    --fileOpenSize_;
    return true;
}

int FileOpenList::addItem(unsigned offset, int flags, int mode, int id)
{
    int i = getFreeItem();
    if(i == -1)
        return -1;
    fileOpenList_[i] = FileOpenItem(offset, flags, mode, id);
    fileOpenSize_++;
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
        return fileOpenList_[id].getId();
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
    if (fileOpenList_[id].getLinkN() == 0){
        deleteItem(id);
        return true;
    }
    return false;
}

UserOpenItem::UserOpenItem(int descriptor, int id) {
    this->descriptor = descriptor;
    this->id = id;
}

void UserOpenItem::clear() {
    descriptor = -1;
    id = -1;
}

int UserOpenItem::getId() {
    return id;
}

int UserOpenItem::getDescriptor() {
    return descriptor;
}

void UserOpenItem::set(int descriptor, int id) {
    this->descriptor = descriptor;
    this->id = id;
}

bool UserOpenItem::check(){
    if (descriptor < 0){
        return false;
    }
    return true;
}

UserOpenList::UserOpenList(std::string username) {
    username_ = username;
}

void UserOpenList::clear() {
    username_ = "";
    for (int i = 0; i < MAX_USER_FD; ++i) {
        iNodeToFile_[i].clear();
    }
}

int UserOpenList::getFileId(int iNodeid) {
    int id = findId(iNodeid);
    if (id == -1){
        return -1;
    }
    return iNodeToFile_[id].getId();
}

bool UserOpenList::addItem(int iNodeId, int fileId){
    int id = findId(iNodeId);
    if (id == -1){
        return false;
    }
    iNodeToFile_[id].set(iNodeId, fileId);
    return true;
}

bool UserOpenList::deleteItem(int iNodeId){
    int id = findId(iNodeId);
    if (id == -1){
        return false;
    }
    iNodeToFile_[id].clear();
    return true;
}

int UserOpenList::findId(int iNodeId) {
    for (int i = 0; i < MAX_USER_FD; ++i) {
        if (iNodeToFile_[i].getDescriptor() == iNodeId){
            return i;
        }
    }
    return -1;
}

int UserOpenList::findFreeItemId() {
    for (int i = 0; i < MAX_USER_FD; ++i) {
        if (iNodeToFile_[i].check() == false){
            return i;
        }
    }
    return -1;
}

std::string UserOpenList::getUsername() {
    return username_;
}