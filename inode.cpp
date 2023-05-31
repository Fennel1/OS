#include "INode.h"

INode::INode(const INode& thx) {
    username_ = thx.username_;
    type_ = thx.type_;
    nlink_ = thx.nlink_;
    file_size_ = thx.file_size_;
    block_num_ = thx.block_num_;
    set_time_ = thx.set_time_;
    mod_time_ = thx.mod_time_;
    indexTable_ = thx.indexTable_;
    dir_ = thx.dir_;
    content_ = thx.content_;
}

INode::INode(std::string username, int type, int nlink, int length, int block_size, std::string set_time, std::string mod_time, std::string content) {
    username_ = username;
    type_ = type;
    nlink_ = nlink;
    file_size_ = file_size_;
    block_num_ = block_num_;
    set_time_ = set_time;
    mod_time_ = mod_time;
    content_ = content;
}

INode& INode::operator=(const INode& thx) {
    username_ = thx.username_;
    type_ = thx.type_;
    nlink_ = thx.nlink_;
    file_size_ = thx.file_size_;
    block_num_ = thx.block_num_;
    set_time_ = thx.set_time_;
    mod_time_ = thx.mod_time_;
    indexTable_ = thx.indexTable_;
    dir_ = thx.dir_;
    content_ = thx.content_;
    return *this;
}

void INode::updateFileSize() {
    if (type_ == 0) {
        file_size_ = content_.size();
    }
    else{
        file_size_ = dir_.getSize() - 2;
    }
}

int INode::getSize() {
    return file_size_;
}

int INode::getBlockNum() {
    return block_num_;
}

int INode::freeBlock() {
    if (block_num_ == 0) {
        return -1;
    }
    int block_id = indexTable_.dropIndex();
    if (block_id == -1) {
        return -1;
    }
    block_num_--;
    return block_id;
}

bool INode::addBlock(int block_id) {
    if (indexTable_.addIndex(block_id)) {
        block_num_++;
        return true;
    }
    return false;
}

int INode::differ()
{
    int block_num = 0;
    if (type_ == 0) {
        block_num = (int)std::ceil((double)content_.size() / (double)BLOCK_SIZE);
    }
    else {
        block_num = dir_.getSize() / 16;
    }
    return block_num - block_num_;
}

std::string INode::getUserName() {
    return username_;
}

void INode::clear() {
    username_ = "";
    type_ = -1;
    nlink_ = 0;
    file_size_ = 0;
    block_num_ = 0;
    set_time_ = "";
    mod_time_ = "";
    indexTable_.clear();
    dir_.clear();
    content_ = "";
}

void INode::addLink() {
    nlink_++;
}

void INode::deleteLink() {
    nlink_--;
}

int INode::getLink() {
    return nlink_;
}

int INode::getType() {
    return type_;
}

bool INode::inodeIsAuthor(std::string username) {
    if (username_ == username || username == "root" || username_ == "root") {
        return true;
    }
    return false;
}

void INode::setModTime(std::string mod_time) {
    mod_time_ = mod_time;
}

Directory *INode::getDir() {
    return &dir_;
}

std::string INode::getSetTime() {
    return set_time_;
}

std::string INode::getModTime() {
    return mod_time_;
}

std::string INode::getTypeInfo(){
    if(type_ == 0){
        return "file";
    }
    else if(type_ == 1){
        return "dir";
    }
    else{
        return "error";
    }
}

INodeList::INodeList() {
    used_size_ = 0;
    for (int i = 0; i < INODE_NUM; i++) {
        iNodeBitMap_[i] = false;
    }
}

int INodeList::getFreeINode() {
    if (used_size_ == INODE_NUM) {
        return -1;
    }
    for (int i = 0; i < INODE_NUM; i++) {
        if (iNodeBitMap_[i] == false) {
            return i;
        }
    }
    return -1;
}

bool INodeList::addINode(INode inode, int pos) {
    if (iNodeBitMap_[pos] == true) {
        return false;
    }
    iNodeBitMap_[pos] = true;
    inode_[pos] = inode;
    used_size_++;
    return true;
}

bool INodeList::deleteINode(int pos) {
    if (iNodeBitMap_[pos] == false) {
        return false;
    }
    iNodeBitMap_[pos] = false;
    inode_[pos].clear();
    used_size_--;
    return true;
}

bool INodeList::updateINode(int pos, INode inode) {
    if (iNodeBitMap_[pos] == false) {
        return false;
    }
    inode_[pos] = inode;
    return true;
}

INode& INodeList::getINode(int pos) {
    return inode_[pos];
}

std::vector<int> INode::getBlockId(){
    return indexTable_.getIndexes();
}

std::vector<int> INodeList::getBlockId(int pos){
    return inode_[pos].getBlockId();
}
