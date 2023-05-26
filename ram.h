#include <string>
#include "settings.h"
#include "inode.h"

#ifndef RAM_H
#define RAM_H


class INodeListInRam {
private:
    const int capacity_ = INODET_IN_RAM; // 内存中i结点表容量
    int size_ = 0; // 内存中i结点表大小
    INode iNodeList[INODET_IN_RAM]; // 内存中i结点表
    int iNodeNum[INODET_IN_RAM]; 

};

class FileOpenItem {
private:
    unsigned offSet = 0;    // 文件偏移量
    int linkN = 0;
    int flags = 0; // 打开文件的参数
    int mode = 0; // 创建文件的参数
    int id = -1; // inode号

public:
};

class FileOpenList {
private:
    const int fileOpenCapacity = MAX_FDS;   // 文件打开表容量
    int fileOpenSize = 0;   // 文件打开表大小
    FileOpenItem fileOpenList[MAX_FDS]; // 文件打开表

public:

};

class UserOpenItem {
private:
    int descriptor = -1; // 其实是文件的磁盘inode号
    int id = -1; // 对应系统文件打开表的下标
public:
};

class UserOpenList{
private:
    std::string username = "";
    UserOpenItem iNodeToFile[MAX_USER_FD];

public:
};

#endif //RAM_H