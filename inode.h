#include <string>
#include <math.h>
#include <fstream>
#include "directory.h"
#include "memory.h"
#include "settings.h"

#ifndef INODE_H
#define INODE_H


class INode {
private:

public:
    std::string username_ = ""; // 用户名
    int type_ = -1; // 文件类型 0-文件 1-目录
    int nlink_ = 0; // 文件链接数
    int file_size_ = 0; // 文件长度
    int block_num_ = 0; // 文件所占磁盘块数
    std::string set_time_ = ""; // 文件创建时间
    std::string mod_time_ = ""; // 文件修改时间
    
    MixIndex indexTable_;    // 索引表
    Directory dir_; // 目录
    std::string content_ = ""; // 文件内容

    INode() = default;
    INode(const INode& thx);
    INode(std::string username, int type, int nlink, int file_size, int block_num, std::string set_time, std::string mod_time, std::string content);
    INode& operator=(const INode& thx);

    void updateFileSize(); // 更新文件大小
    int getSize();  // 获取文件大小
    int getBlockNum();  // 获取文件所占磁盘块数
    int differ();   // 文件变化情况
    int freeBlock();    // 释放文件占用的磁盘块
    bool addBlock(int id);   // 添加磁盘块
    std::string getUserName();  // 获取用户名
    void clear();   // 清空i结点
    void addLink(); // 增加链接数
    void deleteLink();  // 删除链接数
    int getLink();  // 获取链接数
    int getType();  // 获取文件类型
    bool inodeIsAuthor(std::string username); // 判断i结点是否属于某用户
    void setModTime(std::string mod_time); // 设置修改时间
    Directory* getDir();    // 获取目录
    std::string getSetTime(); // 获取创建时间
    std::string getModTime(); // 获取修改时间
    std::string getTypeInfo();  // 获取文件类型
};

class INodeList {
public:
    int used_size_ = 0;     // 已使用的i结点数量
    INode inode_[INODE_NUM];    // i结点
    bool iNodeBitMap_[INODE_NUM];    // i结点位图

    INodeList();

    int getFreeINode(); // 获取空闲i结点
    bool addINode(INode inode, int pos); // 添加i结点
    bool deleteINode(int pos); // 删除i结点
    bool updateINode(int pos, INode inode); // 更新i结点
    INode& getINode(int pos); // 获取i结点
};

#endif //INODE_H