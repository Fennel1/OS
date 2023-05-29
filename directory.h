#include <map>
#include <iostream>

#ifndef DIRECTORY_H
#define DIRECTORY_H

class Directory {
private:

public:
    std::map<std::string, int> directory; // 目录表

    Directory() = default;
    Directory(const Directory& thx);
    Directory& operator=(const Directory& thx);

    bool addItem(std::string filename, int id); // 添加目录项
    bool deleteItem(std::string filename); // 删除目录项
    bool checkItem(std::string filename); // 检查目录项是否存在
    int getItemId(std::string filename); // 获取目录项i结点号
    bool setFileName(std::string filename, std::string newname); // 修改目录项文件名
    int getSize(); // 获取目录项数量
    void init(int id, int parent_id); // 初始化目录项
    void clear();   // 清空目录项
    std::string getFileName(int id); // 获取目录项文件名
    void show(); // 显示目录项
};

#endif //DIRECTORY_H