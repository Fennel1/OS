#include "console.h"

void Console::run() {
    std::string cmd;
    std::vector<std::string> args;
    while (true){
        // 获取当前路径
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

        // 获取指令
        std::getline(std::cin, cmd);
        cmd += " ";
        std::string arg;
        args.clear();
        for (int i = 0; i < cmd.length(); i++){
            if (i > 0 && cmd[i] == ' ' && cmd[i - 1] == ' '){
                continue;
            }
            if (cmd[i] == ' '){
                args.push_back(arg);
                arg = "";
            }
            else{
                arg += cmd[i];
            }
        }

        if (args.size() == 0){
            continue;
        }
        if (args[0] == "create"){
            if (args.size() != 2){
                std::cout << "Usage: create <path>" << std::endl;
                continue;
            }
        }
        else if (args[0] == "delete"){
            if (args.size() != 2){
                std::cout << "Usage: delete <path>" << std::endl;
                continue;
            }
        }
        else if (args[0] == "open"){
            if (args.size() != 2){
                std::cout << "Usage: open <path>" << std::endl;
                continue;
            }
        }
        else if (args[0] == "close"){
            if (args.size() != 2){
                std::cout << "Usage: close <path>" << std::endl;
                continue;
            }
        }
        else if (args[0] == "read"){
            if (args.size() != 2){
                std::cout << "Usage: read <path>" << std::endl;
                continue;
            }
        }
        else if (args[0] == "write"){
            if (args.size() != 2){
                std::cout << "Usage: write <path>" << std::endl;
                continue;
            }
        }
        else if (args[0] == "ls"){
            if (args.size() == 1){
                fileSystem_.ls();
            }
            else if (args.size() == 2){

            }
            else{
                std::cout << "Usage: ls or ls <path>" << std::endl;
                continue;
            }
        }
    }
}