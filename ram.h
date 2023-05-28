#include <string>
#include "settings.h"
#include "inode.h"

#ifndef RAM_H
#define RAM_H


class FileOpenItem {
private:
    unsigned offSet = 0;    // 文件偏移量
    int linkN = 0;
    int sign = 0; // 0-覆盖 1-追加
    int mode = 0; // 0-读 1-写
    int inode_id = -1; // inode号

public:
    FileOpenItem() = default;
    FileOpenItem(unsigned offSet, int flags, int mode, int inode_id);
    FileOpenItem(const FileOpenItem& thx);

    int getINodeId();    // 获取inode号
    int getOffSet();    // 获取偏移量
    bool setOffSet(unsigned offSet);    // 设置偏移量
    void clear();   // 清空
    int getLinkN(); // 获取链接数
    void addLink(); // 增加链接数
    bool deleteLink();  // 删除链接数
    int getMode();  // 获取模式
    int getSign();  // 获取标志
    int setSign(int sign);  // 设置标志
};

class FileOpenList {
private:
    const int capacity_ = MAX_FDS;   // 文件打开表容量
    int size_ = 0;   // 文件打开表大小
    FileOpenItem fileOpenList_[MAX_FDS]; // 文件打开表

public:
    FileOpenList() = default;

    void clear();   // 清空
    int getSize();  // 获取大小
    bool deleteItem(int id); // 删除文件打开表项
    bool deleteLink(int id); // 删除链接数
    bool addLink(int id); // 增加链接数
    int getLinkN(int id);   // 获取链接数
    int getOffSet(int id);  // 获取偏移量
    void setOffSet(int id, unsigned offSet);  // 设置偏移量
    int addItem(unsigned offSet, int sign, int mode, int inode_id); // 添加文件打开表项
    int getItemInodeId(int id); // 获取inode号
    int getFreeItem();  // 获取空闲文件打开表项
    int getMode(int id);    // 获取模式
    int getSign(int id);    // 获取标志
    int setSign(int id, int sign);  // 设置标志
};

class UserOpenItem {
private:
    int inode_id = -1;  // inode号
    int id = -1; // 对应系统文件打开表的下标
public:
    UserOpenItem() = default;
    UserOpenItem(int inode_id, int id);

    void clear();   // 清空
    void set(int inode_id, int id);   // 设置
    int getINodeId();   // 获取inode号
    int getId();    // 获取系统文件打开表的下标
};

class UserOpenList{
private:
    std::string username_ = "";
    UserOpenItem open_list_[MAX_USER_FD];

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