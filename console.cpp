#include "console.h"

void Console::run() {
    std::string cmd;
    while (true){
        std::string path;
        Directory *curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();
        int id = curr_dir->getItemId(".");
        int pid = curr_dir->getItemId("..");
        while (pid != 0){
            path = curr_dir->getFileName(id) + "/" + path;
            id = curr_dir->getItemId(".");
            pid = curr_dir->getItemId("..");
            curr_dir = fileSystem_.superBlock.iNodeList_.inode_[pid].getDir();
        }
        if (pid != id){
            path = "/" + curr_dir->getFileName(id) + "/" + path;
        }
        else{
            path = "/" + path;
        }
        std::cout << fileSystem_.users.curr_user_ << "@OS:" << path << "$ ";
        std::cin >> cmd;
    }
}