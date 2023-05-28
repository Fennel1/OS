#include <string>
#include "settings.h"
#include "inode.h"

#ifndef RAM_H
#define RAM_H


// class INodeListInRam {
// private:
//     const int capacity_ = INODET_IN_RAM; // 内存中i结点表容量
//     int size_ = 0; // 内存中i结点表大小
//     INode iNodeList_[INODET_IN_RAM]; // 内存中i结点表
//     int iNodeNum_[INODET_IN_RAM];    
//     int fileLock_[INODET_IN_RAM];    // 文件锁 1-读锁 2-写锁

// public:
//     INodeListInRam();
//     int getFreeINode(); // 获取空闲i结点
//     bool loadINode(int id, INode iNode); // 加载i结点
//     INode freeINode(int id); // 释放i结点
//     int findINode(int id);  // 查找i结点对应下标
//     INode& getINode(int id); // 获取i结点
//     // bool updateINodeInfo(int id);    // 更新i结点信息
//     // bool saveINodes(); // 保存i结点
//     bool setLock(std::string username, int lock, int id); // 设置锁
// };

class FileOpenItem {
private:
    unsigned offSet = 0;    // 文件偏移量
    int linkN = 0;
    int flags = 0; // 打开文件的参数
    int mode = 0; // 创建文件的参数
    int id = -1; // inode号

public:
    FileOpenItem() = default;
    FileOpenItem(unsigned offSet, int flags, int mode, int id);
    FileOpenItem(const FileOpenItem& thx);

    int getId();    // 获取inode号
    int getOffSet();    // 获取偏移量
    bool setOffSet(unsigned offSet);    // 设置偏移量
    void clear();   // 清空
    int getLinkN(); // 获取链接数
    void addLink(); // 增加链接数
    bool deleteLink();  // 删除链接数
    int getMode();  // 获取创建文件的参数
};

class FileOpenList {
private:
    const int capacity_ = MAX_FDS;   // 文件打开表容量
    int fileOpenSize_ = 0;   // 文件打开表大小
    FileOpenItem fileOpenList_[MAX_FDS]; // 文件打开表

public:
    FileOpenList() = default;

    void clear();   // 清空
    int getSize();  // 获取大小
    bool deleteItem(int id); // 删除文件打开表项
    bool deleteLink(int id); // 删除链接数
    bool addLink(int id); // 增加链接数
    int getOffSet(int id);  // 获取偏移量
    void setOffSet(int id, unsigned offSet);  // 设置偏移量
    int addItem(unsigned offSet, int flags, int mode, int id); // 添加文件打开表项
    int getItemInodeId(int id); // 获取inode号
    int getFreeItem();  // 获取空闲文件打开表项
    int getMode(int id);    // 获取模式
};

class UserOpenItem {
private:
    int descriptor = -1; // 其实是文件的磁盘inode号
    int id = -1; // 对应系统文件打开表的下标
public:
    UserOpenItem() = default;
    UserOpenItem(int descriptor, int id);

    void clear();   // 清空
    void set(int descriptor, int id);   // 设置
    int getDescriptor();    // 获取文件描述符
    int getId();    // 获取系统文件打开表的下标
    bool check();   // 检查是否可用
};

class UserOpenList{
private:
    std::string username_ = "";
    UserOpenItem iNodeToFile_[MAX_USER_FD];

public:
    UserOpenList() = default;
    UserOpenList(std::string username);

    void clear();   // 清空
    int getFileId(int iNodeid); // 获取文件打开表下标
    bool addItem(int iNodeId, int fileId);  // 添加文件描述符
    bool deleteItem(int iNodeId);    // 删除文件描述符
    int findId(int iNodeId);    // 查找下标
    int findFreeItemId();   // 查找空闲下标
    std::string getUsername();  // 获取用户名
};

#endif //RAM_H