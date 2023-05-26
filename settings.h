#ifndef SETTINGS_H
#define SETTINGS_H

#define BLOCK_SIZE 64 // 磁盘块大小
#define BASIC_IDX 10 // 混合索引表中直接索引的数量
#define IDXT_SIZE (BLOCK_SIZE/4) // index table size 普通索引表大小 16
#define ONE_IDXT_SIZE (IDXT_SIZE*IDXT_SIZE) // 二次间接索引表大小 256
#define TWO_IDXT_SIZE (ONE_IDXT_SIZE*IDXT_SIZE) // 三次间接索引表大小 4096
#define MIX_IDXT_SIZE (BASIC_IDX+IDXT_SIZE+ONE_IDXT_SIZE+TWO_IDXT_SIZE) // 混合索引表大小 4418

#define INODE_NUM 128    //i结点个数
#define INODET_IN_RAM 64 // 内存中i结点表可容纳i结点的最大数量
#define MAX_FDS 64  // 系统文件打开表最大数量
#define MAX_USER_FD 5 // 用户文件打开表最大数量

#define GROUP_SIZE 50 // 成组链接法每一组的大小
#define TOTAL_GROUP_SIZE 500 // 成组链接法初始化时的总大小

#endif