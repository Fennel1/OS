#include <iostream>
#include <time.h>
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

public:
    INodeList iNodeList_;   // i结点表
    SuperGroup superGroup_; // 组
    SuperBlock();

    bool createFile(std::string filename, Directory* cur_dir, std::string curr_user); // 创建文件
    void deleteFile(std::string filename, Directory* cur_dir); // 删除文件
    void createDir(std::string dirname, INode &dir, Directory* cur_dir, int pos); // 创建目录
    void deleteDir(std::string dirname, INode &dir, Directory* cur_dir, int pos); // 删除目录
};

class FileSystem {
public:
    SuperBlock superBlock;  // 超级块
    INodeList iNodeList;  // 内存中i结点表

    Users users;

    FileOpenList fileOpenList;
    std::map<std::string, UserOpenList> userOpenList;

    void createFile(std::string filename);  // 创建文件
    void deleteFile(std::string filename);  // 删除文件
};



#endif // FILESYSTEM_H