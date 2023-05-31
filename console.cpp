#include "console.h"
#include<conio.h>
void Console::run() {
    std::string cmd;
    std::vector<std::string> args;
    while (true){

        fileSystem_.is_error = false;
        writeMessage();
        printPath();
        Directory *curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();

        // 获取指令, 并分割参数
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
            bool is_root, is_file;
            std::vector<std::string> path_list = splitPath(args[1], is_root, is_file);
            // 计算路径
            if (is_root){
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[0].getDir();
            }
            else{
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();   
            }
            // 创建文件/目录
            if (is_file){       // 创建文件
                for (int i=0; i<path_list.size()-1; i++){
                    int id = curr_dir->getItemId(path_list[i]);
                    if (id == -1){
                        fileSystem_.createDir(path_list[i], curr_dir);
                        id = curr_dir->getItemId(path_list[i]);
                    }
                    if (fileSystem_.superBlock.iNodeList_.inode_[id].getType() == 0){
                        fileSystem_.is_error = true;
                        fileSystem_.error_msg = "Error: " + path_list[i] + " is a file";
                        break;
                    }
                    curr_dir = fileSystem_.superBlock.iNodeList_.inode_[id].getDir();
                }
                if (!fileSystem_.is_error){
                    fileSystem_.createFile(path_list[path_list.size()-1], curr_dir);
                }
            }
            else{        // 创建目录
                for (int i=0; i<path_list.size(); i++){
                    int id = curr_dir->getItemId(path_list[i]);
                    if (id == -1){
                        fileSystem_.createDir(path_list[i], curr_dir);
                        id = curr_dir->getItemId(path_list[i]);
                    }
                    if (fileSystem_.superBlock.iNodeList_.inode_[id].getType() == 0){
                        fileSystem_.is_error = true;
                        fileSystem_.error_msg = "Error: " + path_list[i] + " is a file";
                        break;
                    }
                    curr_dir = fileSystem_.superBlock.iNodeList_.inode_[id].getDir();
                }
            }
        }
        else if (args[0] == "delete"){
            if (args.size() != 2){
                std::cout << "Usage: delete <path>" << std::endl;
                continue;
            }
            bool is_root, is_file;
            std::vector<std::string> path_list = splitPath(args[1], is_root, is_file);
            // 计算路径
            if (is_root){
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[0].getDir();
            }
            else{
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();   
            }
            // 删除文件/目录
            if (is_file){       // 删除文件
                for (int i=0; i<path_list.size()-1; i++){
                    int id = curr_dir->getItemId(path_list[i]);
                    if (id == -1){
                        fileSystem_.is_error = true;
                        fileSystem_.error_msg = "Error: " + path_list[i] + " is not exist.";
                        break;
                    }
                    if (fileSystem_.superBlock.iNodeList_.inode_[id].getType() == 0){
                        fileSystem_.is_error = true;
                        fileSystem_.error_msg = "Error: " + path_list[i] + " is a file";
                        break;
                    }
                    curr_dir = fileSystem_.superBlock.iNodeList_.inode_[id].getDir();
                }
                // std::cout << "delete file: " << path_list[path_list.size()-1] << std::endl;
                fileSystem_.deleteFile(path_list[path_list.size()-1], curr_dir);
            }
            else{        // 删除目录
                for (int i=0; i<path_list.size()-1; i++){
                    int id = curr_dir->getItemId(path_list[i]);
                    if (id == -1){
                        fileSystem_.is_error = true;
                        fileSystem_.error_msg = "Error: " + path_list[i] + " is not exist.";
                        break;
                    }
                    if (fileSystem_.superBlock.iNodeList_.inode_[id].getType() == 0){
                        fileSystem_.is_error = true;
                        fileSystem_.error_msg = "Error: " + path_list[i] + " is a file";
                        break;
                    }
                    curr_dir = fileSystem_.superBlock.iNodeList_.inode_[id].getDir();
                }
                if (!fileSystem_.is_error){
                    fileSystem_.deleteDir(path_list[path_list.size()-1], curr_dir);
                }
            }
        }
        else if (args[0] == "open"){
            if (args.size() != 3){
                std::cout << "Usage: open <path> <mode>" << std::endl;
                continue;
            }
            bool is_root, is_file;
            std::vector<std::string> path_list = splitPath(args[1], is_root, is_file);
            if (is_file == false){
                fileSystem_.is_error = true;
                fileSystem_.error_msg = "Error: " + args[1] + " is a directory";
                std::cout << fileSystem_.error_msg << std::endl;
                continue;
            }
            // 计算路径
            if (is_root){
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[0].getDir();
            }
            else{
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();   
            }
            // 打开文件
            for (int i=0; i<path_list.size()-1; i++){
                int id = curr_dir->getItemId(path_list[i]);
                if (id == -1){
                    fileSystem_.is_error = true;
                    fileSystem_.error_msg = "Error: " + path_list[i] + " is not exist.";
                    break;
                }
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[id].getDir();
            }
            if (!fileSystem_.is_error){
                fileSystem_.openFile(path_list[path_list.size()-1], std::stoi(args[2]), 0, curr_dir);
            }
        }
        else if (args[0] == "close"){
            if (args.size() != 2){
                std::cout << "Usage: close <path>" << std::endl;
                continue;
            }
            bool is_root, is_file;
            std::vector<std::string> path_list = splitPath(args[1], is_root, is_file);
            if (is_file == false){
                fileSystem_.is_error = true;
                fileSystem_.error_msg = "Error: " + args[1] + " is a directory";
                std::cout << fileSystem_.error_msg << std::endl;
                continue;
            }
            // 计算路径
            if (is_root){
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[0].getDir();
            }
            else{
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();   
            }
            // 关闭文件
            for (int i=0; i<path_list.size()-1; i++){
                int id = curr_dir->getItemId(path_list[i]);
                if (id == -1){
                    fileSystem_.is_error = true;
                    fileSystem_.error_msg = "Error: " + path_list[i] + " is not exist.";
                    break;
                }
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[id].getDir();
            }
            if (!fileSystem_.is_error){
                fileSystem_.closeFile(path_list[path_list.size()-1], curr_dir);
            }
        }
        else if (args[0] == "read"){
            if (args.size() != 2){
                std::cout << "Usage: read <path>" << std::endl;
                continue;
            }
            bool is_root, is_file;
            std::vector<std::string>path_list=splitPath(args[1],is_root,is_file);
            // 计算路径
            if (is_root){
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[0].getDir();
            }
            else{
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();   
            }
             //输出文件内容到终端
            std::string f = fileSystem_.readFile(path_list[path_list.size()-1], 0);
            if (f == ""){
                std::cout << "Error: " << args[1] << std::endl;
                continue;
            }
            system("cls");//清空终端
            std::cout<<f;
            getchar();
        }
        else if (args[0] == "write"){
            if (args.size() != 2){
                std::cout << "Usage: write <path>" << std::endl;
                continue;
            }
            bool is_root, is_file;
            std::vector<std::string>path_list=splitPath(args[1],is_root,is_file);
            // 计算路径
            if (is_root){
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[0].getDir();
            }
            else{
                curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();   
            }
            std::string f="";
            char a;
            // system("cls");
            f = fileSystem_.readFile(path_list[path_list.size()-1], 0);
            if (f == ""){
                std::cout << "Error: " << args[1] << std::endl;
                continue;
            }
            while(1)
            {
                system("cls");
                // std::cout << 1 << std::endl;
                std::cout<<f;
                a=getche();
                if(a=='\b')
                {
                    if (f.size()){
                        f.pop_back();
                    }
                }
                else if(a==27)
                {
                    break;
                }
                else if(a=='\r')
                {
                    f.push_back('\n');
                }
                else
                {
                    f.push_back(a);
                }
            }
            if (fileSystem_.writeFile(path_list[path_list.size()-1], f)){
                system("cls");
                std::cout<<path_list[path_list.size()-1]<<" has been written."<<std::endl;
            }
            else {
                system("cls");
                std::cout<<"Error: "<<path_list[path_list.size()-1]<<" can't be written."<<std::endl;
            }
        }
        else if (args[0] == "cd"){
            if (args.size() != 2){
                std::cout << "Usage: cd <path>" << std::endl;
                continue;
            }
            bool is_root, is_file;
            std::vector<std::string> path_list = splitPath(args[1], is_root, is_file);
            //计算路径
            if(is_root){
                fileSystem_.users.setInodeId(0);
            }
            for (int i=0; i<path_list.size(); i++){
                fileSystem_.cd(path_list[i]);
            }
        }
        else if (args[0] == "ls"){
            // TODO 支持ls <path>格式，包括相对路径、绝对路径，输出全部信息 邓
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
        else if (args[0] == "rm"){
            if (args.size() != 2){
                std::cout << "Usage: rm <path>" << std::endl;
                continue;
            }
            // TODO 支持递归删除目录 

        }
        else if (args[0] == "login"){
            if (args.size() != 3){
                std::cout << "Usage: login <username> <password>" << std::endl;
                continue;
            }
            if (fileSystem_.users.login(args[1], args[2]) == false){
                std::cout << "Error: username or password is wrong." << std::endl;
                continue;
            }
        }
        else if (args[0] == "logout"){
            if (args.size() != 1){
                std::cout << "Usage: logout" << std::endl;
                continue;
            }
            fileSystem_.users.logOut();
        }
        else if (args[0] == "adduser"){
            if (args.size() != 3){
                std::cout << "Usage: adduser <username> <password>" << std::endl;
                continue;
            }
            if (fileSystem_.users.isExist(args[1])){
                std::cout << "Error: " << args[1] << " is exist." << std::endl;
                continue;
            }
            fileSystem_.users.createUser(args[1], args[2]);
        }
        else if (args[0] == "switch"){
            if (args.size() != 2){
                std::cout << "Usage: switch <username>" << std::endl;
                continue;
            }
            if (fileSystem_.users.switchUser(args[1]) == false){
                std::cout << "Error: " << args[1] << " is not exist or not login." << std::endl;
                continue;
            }
        }
        else if (args[0] == "format"){
            if (args.size() != 1){
                std::cout << "Usage: format" << std::endl;
                continue;
            }
            fileSystem_.format();
        }
        else if (args[0] == "save"){
            if (args.size() != 1){
                std::cout << "Usage: save" << std::endl;
                continue;
            }
            fileSystem_.save();
        }
        else if (args[0] == "load"){
            if (args.size() != 1){
                std::cout << "Usage: load" << std::endl;
                continue;
            }
            fileSystem_.load();
        }
        else if (args[0] == "rename"){
            if (args.size() != 3){
                std::cout << "Usage: rename <oldname> <newname>" << std::endl;
                continue;
            }
            curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();
            if (!curr_dir->setFileName(args[1], args[2])){
                std::cout << "Error: " << args[1] << " is not exist or " << args[2] << " is exist." << std::endl;
                continue;
            }
        }
        else if (args[0] == "help"){
            if (args.size() != 1){
                std::cout << "Usage: help" << std::endl;
                continue;
            }
            help_.help();
        }
        else if (args[0] == "man"){
            if (args.size() != 2){
                std::cout << "Usage: man <command>" << std::endl;
                continue;
            }
            help_.man(args[1]);
        }
        else {
            std::cout << "Error: " << args[0] << " is not a command." << std::endl;
            continue;
        }

        if (fileSystem_.is_error){
            std::cout << "Error: " << fileSystem_.error_msg << std::endl;
        }
    }
}

std::vector<std::string> Console::splitPath(std::string path, bool &is_root, bool &is_file){
    //判断是否为绝对路径
    is_root = path[0] == '/';
    is_file = path[path.length() - 1] != '/';
    if(path[0] == '/'){
        path = path.substr(1);
    }

    std::vector<std::string> path_list;
    std::string item;
    path += "/";
    for(int i = 0; i < path.length(); i++){
        if(path[i] == '/' && item != ""){
            path_list.push_back(item);
            item = "";
        }
        else{
            item += path[i];
        }
    }
    return path_list;
}

void Console::printPath(){
    std::string path;
    Directory *curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();
    int id = curr_dir->getItemId(".");
    int pid = curr_dir->getItemId("..");
    while (pid != id){
        // std::cout << id << " " << pid << std::endl;
        curr_dir = fileSystem_.superBlock.iNodeList_.inode_[pid].getDir();
        path = curr_dir->getFileName(id) + "/" + path;
        id = curr_dir->getItemId(".");
        pid = curr_dir->getItemId("..");
    }
    path = "/" + path;
    std::cout << fileSystem_.users.curr_user_ << "@OS:" << path << "$ ";
}

void Console::writeMessage() {
    Directory *curr_dir = fileSystem_.superBlock.iNodeList_.inode_[fileSystem_.users.getInodeId()].getDir();
    std::ofstream msg("..\\debug\\message.txt");
    for (auto iter = curr_dir->directory.begin(); iter != curr_dir->directory.end(); iter++){
        msg << iter->first << " " << iter->second;
        if (fileSystem_.superBlock.iNodeList_.inode_[iter->second].getType() == 0){
            std::vector<int> block_list = fileSystem_.superBlock.iNodeList_.inode_[iter->second].getBlockId();
            msg << " num of block: " << block_list.size() << " list: ";
            for (auto iter2 = block_list.begin(); iter2 != block_list.end(); iter2++){
                msg << *iter2 << " ";
            }
        }
        msg << std::endl;
    }
    msg.close();
}