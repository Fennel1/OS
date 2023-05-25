#include <vector>
#include <string>
#include <map>
#include <stack>

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define BLOCK_SIZE 64 // 磁盘块大小
#define BASIC_IDX 10 // 混合索引表中直接索引的数量
#define IDXT_SIZE (BLOCK_SIZE/4) // index table size 普通索引表大小
#define ONE_IDXT_SIZE (IDXT_SIZE*IDXT_SIZE) // 二次间接索引表大小
#define TWO_IDXT_SIZE (ONE_IDXT_SIZE*IDXT_SIZE) // 三次间接索引表大小
#define MIX_IDXT_SIZE (BASIC_IDX+IDXT_SIZE+ONE_IDXT_SIZE+TWO_IDXT_SIZE) // 混合索引表大小

#define INODE_NUM 128    //i结点个数
#define INODET_IN_RAM 64 // 内存中i结点表可容纳i结点的最大数量
#define MAX_FDS 64  // 系统文件打开表最大数量
#define MAX_USER_FD 5 // 用户文件打开表最大数量

#define GROUP_SIZE 50 // 成组链接法每一组的大小
#define TOTAL_GROUP_SIZE 500 // 成组链接法初始化时的总大小

class NormalIndex {
private:
    int capacity_ = IDXT_SIZE; // 普通索引表容量
    int sign_; // 普通索引表标记
    int size_ = 0; // 普通索引表大小
    std::vector<int> index_; // 普通索引表
    std::vector<NormalIndex> next_; // 普通索引表的下一级索引表

public:
    NormalIndex();
};

class MixIndex {
private:
    int capacity_ = MIX_IDXT_SIZE; // 混合索引表容量
    int size_ = 0; // 混合索引表大小
    NormalIndex oneIndirectIndex;
    NormalIndex twoIndirectIndex;
    NormalIndex threeIndirectIndex;

public:
    MixIndex();
};

class Directory {
private:
    std::map<std::string, int> directory; // 目录表

public:
    Directory();
};

class INode {
private:
    std::string name_ = ""; // 文件名
    int type_ = -1; // 文件类型
    int nlink_ = 0; // 文件链接数
    int length_ = 0; // 文件长度
    int block_size_ = 0; // 文件所占磁盘块数
    std::string set_time_ = ""; // 文件创建时间
    std::string mod_time_ = ""; // 文件修改时间

    MixIndex indexT;    // 索引表

public:
    Directory directory; // 目录
    std::string content; // 文件内容

public:
    INode();
};

class INodeList {
public:
    int used_size_ = 0;     // 已使用的i结点数量
    INode inode_[INODE_NUM];    // i结点
};

class GroupLeader {
private:
    int capacity_ = GROUP_SIZE; // 组容量
    int size_ = 0; // 组大小
    GroupLeader *next_leader_; // 下一组
    std::stack<int> groups_; // 组

public:
    GroupLeader();
};

class SuperGroup {
private:
    GroupLeader *curr_group_; // 当前组
    int free_groupNum_  = TOTAL_GROUP_SIZE; // 空闲组数
public:
};

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

class FileSystem {
public:
    SuperBlock superBlock;  // 超级块
    INodeListInRam iNodeListInRam;  // 内存中i结点表

    FileOpenList fileOpenList;
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



#endif // FILESYSTEM_H