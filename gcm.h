#include <stack>
#include "settings.h"

#ifndef GCM_H
#define GCM_H


class GroupLeader {
private:
    int capacity_ = GROUP_SIZE; // 组容量
    int size_ = 0; // 组大小
    GroupLeader *next_leader_; // 下一组
    std::stack<int> groups_; // 组

public:
    GroupLeader();
    GroupLeader(GroupLeader *next_leader);
    GroupLeader(const GroupLeader &thx);

    int getCapacity() const; // 获取组容量
    int getSize() const;    // 获取当前组大小
    int getBlockId();         // 获取当前组中的一个空闲块号
    bool addBlock(int block_id);    // 向当前组中添加一个块
    GroupLeader* getLeader(); // 获取当前组
    GroupLeader* getNextLeader() const; // 获取下一组
    bool setNextLeader(GroupLeader *next_leader); // 设置组长块
};

class SuperGroup {
private:
    GroupLeader *curr_group_; // 当前组
    int free_groupNum_ = TOTAL_GROUP_SIZE; // 空闲组数
public:
    int getFreeGroup(); // 获取一个空闲块
    bool addBlock(int block_id); // 向当前组中添加一个块
    int getSize() const; // 获取剩余总块数
    void init();
};

#endif //GCM_H