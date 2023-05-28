#include <iostream>
#include <time.h>
#include "settings.h"
#include "inode.h"
#include "memory.h"
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "user.h"
#include "ram.h"
#include "gcm.h"
#include "directory.h"

#ifndef FILESYSTEM_H
#define FILESYSTEM_H


class SuperBlock {
private:


public:
    INodeList iNodeList_;   // i结点表
    SuperBlock();

    void createFile(std::string filename, Directory* cur_dir, std::string curr_user); // 创建文件
    void deleteFile(std::string filename, Directory* cur_dir, std::string curr_user); // 删除文件
    void createDir(std::string dirname, Directory* cur_dir, std::string curr_user); // 创建目录
    void deleteDir(std::string dirname, Directory* cur_dir, std::string curr_user); // 删除目录
};

class FileSystem {
public:
    SuperBlock superBlock;      // 超级块
    SuperGroup superGroup;      // 组
    Users users;                // 用户
    FileOpenList fileOpenList;  // 文件打开表
    std::map<std::string, UserOpenList> userOpenList;   // 用户打开表

    FileSystem();

    void createFile(std::string filename, Directory* curr_dir);  // 创建文件
    void deleteFile(std::string filename, Directory* curr_dir);  // 删除文件
    void createDir(std::string dirname, Directory* curr_dir);    // 创建目录
    void deleteDir(std::string dirname, Directory* curr_dir);    // 删除目录
    void cd(std::string dirname);    // 切换目录
    bool openFile(std::string filename, int mode, int sign, Directory* curr_dir);    // 打开文件 mode: 0-读 1-写 sign: 0-覆盖 1-追加
    bool closeFile(std::string filename, Directory* curr_dir);   // 关闭文件
    bool writeFile(std::string filename, std::string content);   // 写文件
    std::string readFile(std::string filename, int len);  // 读文件
    void ls();  // 显示当前目录下的文件
    void ls(std::string path);   // 显示指定目录下的文件
};



#endif // FILESYSTEM_H