#include <vector>
#include "settings.h"

#ifndef MEMORY_H
#define MEMORY_H


class NormalIndex {
private:
    int capacity_ = IDXT_SIZE; // 普通索引表容量
    int sign_; // 普通索引表标记
    int size_ = 0; // 普通索引表大小
    std::vector<int> indexes_; // 普通索引表
    std::vector<NormalIndex> indexTables_; // 普通索引表的下一级索引表

public:
    NormalIndex(int sign = 0);
    NormalIndex(const NormalIndex& thx);
    NormalIndex& operator=(const NormalIndex& thx);

    int getIndex(int n);    // 获取第n个索引
    bool addIndex(int id);  // 添加磁盘块号为id的新索引
    int getSize() const;    // 获取索引表大小
    bool dropIndex(int n);  //  删除n个索引
    int dropIndex();        // 删除最后一个索引

    NormalIndex& getNextIndex(int n);    // 获取第n个下一级索引表
    bool addNextIndex();   // 添加下一级索引表
    bool dropNextIndex(int n);  // 删除n个下一级索引表
    bool dropNextIndex();   // 删除最后一个下一级索引表
    int getSign() const;    // 获取标记
    void clear();    // 清空索引表
    std::vector<int> getIndexes() const;  // 获取索引表
};

class MixIndex {
private:
    int capacity_ = MIX_IDXT_SIZE; // 混合索引表容量
    int size_ = 0; // 混合索引表大小
    std::vector<int> indexes_;      // 直接索引表
    NormalIndex oneIndirectIndex;   // 一级间接索引表
    NormalIndex twoIndirectIndex;   // 二级间接索引表
    NormalIndex threeIndirectIndex; // 三级间接索引表

    bool addDirectIndex(int id);    // 添加直接索引
    bool addOneIndirectIndex(int id);   // 添加一级间接索引
    bool addTwoIndirectIndex(int id);   // 添加二级间接索引
    bool addThreeIndirectIndex(int id); // 添加三级间接索引
    int dropDirectIndex();  // 删除直接索引
    int dropOneIndirectIndex(); // 删除一级间接索引
    int dropTwoIndirectIndex(); // 删除二级间接索引
    int dropThreeIndirectIndex();   // 删除三级间接索引

public:
    MixIndex();
    MixIndex(const MixIndex& thx);
    MixIndex& operator=(const MixIndex& thx);

    bool addIndex(int id);  // 添加新索引
    int dropIndex();        // 删除最后一个索引
    int getSize() const;    // 获取索引表大小
    void clear();           // 清空索引表
    std::vector<int> getIndexes();  // 获取索引表

};

#endif //MEMORY_H