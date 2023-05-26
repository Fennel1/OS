#include "settings.h"
#include "inode.h"
#include "memory.h"
#include "user.h"
#include "ram.h"
#include "gcm.h"
#include "directory.h"

#ifndef FILESYSTEM_H
#define FILESYSTEM_H


class SuperBlock {
private:
    int inodeNum_ = INODE_NUM;      // i结点总数
    int free_inodeNum_ = INODE_NUM; // 空闲i结点数
    int blockNum_ = TOTAL_GROUP_SIZE;      // 磁盘块总数
    int free_blockNum_ = TOTAL_GROUP_SIZE; // 空闲磁盘块数

    INodeList inodeList_;   // i结点表
    SuperGroup superGroup_; // 组

public:
    SuperBlock();
};

class FileSystem {
public:
    SuperBlock superBlock;  // 超级块
    INodeListInRam iNodeListInRam;  // 内存中i结点表

    Users users;

    FileOpenList fileOpenList;
    std::map<std::string, UserOpenList> userOpenList;
};



#endif // FILESYSTEM_H