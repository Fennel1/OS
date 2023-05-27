#include "INode.h"

bool iNodeBitMap[INODE_NUM];

INode::INode(const INode& thx) {
    username_ = thx.username_;
    type_ = thx.type_;
    nlink_ = thx.nlink_;
    length_ = thx.length_;
    block_size_ = thx.block_size_;
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
    length_ = length;
    block_size_ = block_size;
    set_time_ = set_time;
    mod_time_ = mod_time;
    content_ = content;
}

INode& INode::operator=(const INode& thx) {
    username_ = thx.username_;
    type_ = thx.type_;
    nlink_ = thx.nlink_;
    length_ = thx.length_;
    block_size_ = thx.block_size_;
    set_time_ = thx.set_time_;
    mod_time_ = thx.mod_time_;
    indexTable_ = thx.indexTable_;
    dir_ = thx.dir_;
    content_ = thx.content_;
    return *this;
}

void INode::updateFileSize() {
    if (type_ == 0) {
        length_ = sizeof(content_);
    }
    else{
        length_ = sizeof(dir_);
    }
}

int INode::getSize() {
    return length_;
}

int INode::getBlockNum() {
    return block_size_;
}

int INode::freeBlock() {
    return indexTable_.dropIndex();
}

bool INode::addBlock(int id) {
    if (indexTable_.addIndex(id)) {
        return true;
    }
    return false;
}

int INode::differ()
{
    int t;
    if(type_ == 1){
        t = sizeof(dir_) - length_;
    }
    else{
        t = sizeof(content_) - length_;
    }
    return (int)std::ceil((double)t / (double)BLOCK_SIZE);
}

std::string INode::getUserName() {
    return username_;
}

void INode::clear() {
    username_ = "";
    type_ = -1;
    nlink_ = 0;
    length_ = 0;
    block_size_ = 0;
    set_time_ = "";
    mod_time_ = "";
    indexTable_.clear();
    dir_.clear();
    content_ = "";
}

void INode::addLink() {
    nlink_++;
}

bool INode::deleteLink() {
    nlink_--;
    if (nlink_ == 0) {
        return true;
    }
    return false;
}

int INode::getLink() {
    return nlink_;
}

int INode::getType() {
    return type_;
}

bool INode::inodeIsAuthor(std::string username) {
    if (username_ == username) {
        return true;
    }
    return false;
}

int INodeList::getFreeINode() {
    for (int i = 0; i < INODE_NUM; i++) {
        if (iNodeBitMap[i] == false) {
            // iNodeBitMap[i] = true;
            return i;
        }
    }
    return -1;
}

bool INodeList::addINode(INode inode, int pos) {
    if (iNodeBitMap[pos] == true) {
        return false;
    }
    iNodeBitMap[pos] = true;
    inode_[pos] = inode;
    used_size_++;
    return true;
}

void INodeList::deleteINode(int pos) {
    iNodeBitMap[pos] = false;
    inode_[pos].clear();
    used_size_--;
}

void INodeList::updateINode(int pos, INode inode) {
    inode_[pos] = inode;
}

INode& INodeList::getINode(int pos) {
    return inode_[pos];
}