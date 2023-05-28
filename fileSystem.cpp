#include "filesystem.h"

SuperBlock::SuperBlock() {}

void SuperBlock::createFile(std::string filename, Directory* cur_dir, std::string curr_user){
    // 获取空闲inode
    int inode_id = iNodeList_.getFreeINode();
    if (inode_id == -1){
        std::cout << "No free inode!" << std::endl;
        return ;
    }
    // 判断是否有权限
    if (!iNodeList_.inode_[inode_id].inodeIsAuthor(curr_user)){
        std::cout << "You are not the author!" << std::endl;
        return ;
    }
    // 获取当前时间
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    std::string curr_time = ss.str();

    // 创建inode, 并添加到inode表中
    iNodeList_.addINode(INode(curr_user, 0, 0, 0, 0, curr_time, curr_time, filename), inode_id);
    // 添加到目录中
    cur_dir->addItem(filename, inode_id);
}

void FileSystem::createFile(std::string filename) {
    superBlock.createFile(filename, superBlock.iNodeList_.inode_[users.getInodeId()].getDir(), users.curr_user_);
}

void SuperBlock::deleteFile(std::string filename, Directory* cur_dir, std::string curr_user){
    // 获取inode
    int inode_id = cur_dir->getItemId(filename);
    if (inode_id == -1){
        std::cout << "No such file!" << std::endl;
        return;
    }
    // 判断是否有权限
    if (!iNodeList_.inode_[inode_id].inodeIsAuthor(curr_user)){
        std::cout << "You are not the author!" << std::endl;
        return ;
    }
    // 判断是否为文件
    if (iNodeList_.inode_[inode_id].getType() == 1){
        std::cout << "It's a directory!" << std::endl;
        return;
    }
    // 删除inode
    if (iNodeList_.deleteINode(inode_id)){
        std::cout << "Delete file successfully!" << std::endl;
    }
    else{
        std::cout << "Delete file failed!" << std::endl;
    }
}

void FileSystem::deleteFile(std::string filename) {
    superBlock.deleteFile(filename, superBlock.iNodeList_.inode_[users.getInodeId()].getDir(), users.curr_user_);
}

void SuperBlock::createDir(std::string dirname, Directory* cur_dir, std::string curr_user){
    // 获取空闲inode
    int inode_id = iNodeList_.getFreeINode();
    if (inode_id == -1){
        std::cout << "No free inode!" << std::endl;
        return ;
    }
    // 判断是否有权限
    if (!iNodeList_.inode_[inode_id].inodeIsAuthor(curr_user)){
        std::cout << "You are not the author!" << std::endl;
        return ;
    }
    // 获取当前时间
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    std::string curr_time = ss.str();

    // 创建inode, 并添加到inode表中
    iNodeList_.addINode(INode(curr_user, 1, 0, 0, 0, curr_time, curr_time, dirname), inode_id);
    // 添加到目录中
    cur_dir->addItem(dirname, inode_id);
}

void FileSystem::createDir(std::string dirname) {
    superBlock.createDir(dirname, superBlock.iNodeList_.inode_[users.getInodeId()].getDir(), users.curr_user_);
}

void SuperBlock::deleteDir(std::string dirname, Directory* cur_dir, std::string curr_user){
    // 获取inode
    int inode_id = cur_dir->getItemId(dirname);
    if (inode_id == -1){
        std::cout << "No such directory!" << std::endl;
        return;
    }
    // 判断是否有权限
    if (!iNodeList_.inode_[inode_id].inodeIsAuthor(curr_user)){
        std::cout << "You are not the author!" << std::endl;
        return ;
    }
    // 判断是否为目录
    if (iNodeList_.inode_[inode_id].getType() == 0){
        std::cout << "It's a file!" << std::endl;
        return;
    }
    // 判断目录是否为空
    if (iNodeList_.inode_[inode_id].dir_.getSize() != 0){
        std::cout << "Directory is not empty!" << std::endl;
        return;
    }
    // 删除inode
    if (iNodeList_.deleteINode(inode_id)){
        std::cout << "Delete directory successfully!" << std::endl;
    }
    else{
        std::cout << "Delete directory failed!" << std::endl;
    }
}

void FileSystem::deleteDir(std::string dirname) { 
    superBlock.deleteDir(dirname, superBlock.iNodeList_.inode_[users.getInodeId()].getDir(), users.curr_user_);
}

FileSystem::FileSystem() {
    // 初始化
    superGroup.init();
}

void FileSystem::cd(std::string dirname) {
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(dirname);
    if (inode_id == -1){
        std::cout << "No such directory! " << dirname << std::endl;
        return;
    }
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 0){
        std::cout << "It's a file! " << dirname << std::endl;
        return;
    }
    users.setInodeId(inode_id);
}

bool FileSystem::openFile(std::string filename, int mode, int sign) {
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        std::cout << "No such file!" << filename << std::endl;
        return false;
    }
    // 判断是否有权限
    if (!superBlock.iNodeList_.inode_[inode_id].inodeIsAuthor(users.curr_user_)){
        std::cout << "You are not the author!" << std::endl;
        return false;
    }
    
    // 判断是否为文件/目录
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 1){
        cd(filename);
        return true;
    }
    else{
        // 添加到打开文件表中
        int file_id;
        if (sign == 0){
            file_id = fileOpenList.addItem(0, sign, mode, inode_id);
        }
        else{
            file_id = fileOpenList.addItem(superBlock.iNodeList_.inode_[inode_id].content_.size(), sign, mode, inode_id);
        }
        // 添加链接
        fileOpenList.addLink(file_id);
        superBlock.iNodeList_.inode_[inode_id].addLink();

        // 添加到用户打开文件表中
        if (userOpenList.find(users.curr_user_) == userOpenList.end()){
            userOpenList[users.curr_user_] = UserOpenList(users.curr_user_);
        }
        if (userOpenList[users.curr_user_].addItem(inode_id, file_id)){
            std::cout << "Open file successfully!" << std::endl;
            return true;
        }
        else{
            std::cout << "Open file failed!" << std::endl;
            return false;
        }
    }
}

bool FileSystem::closeFile(std::string filename){
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        std::cout << "No such file!" << filename << std::endl;
        return false;
    }
    // 判断是否有权限
    if (!superBlock.iNodeList_.inode_[inode_id].inodeIsAuthor(users.curr_user_)){
        std::cout << "You are not the author!" << std::endl;
        return false;
    }
    // 判断是否为文件
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 1){
        std::cout << "It's a directory!" << std::endl;
        return false;
    }

    int file_id = userOpenList[users.curr_user_].getFileId(inode_id);
    // 删除用户打开文件表中的项
    userOpenList[users.curr_user_].deleteItem(inode_id);
    // 删除打开文件表中的项
    fileOpenList.deleteLink(file_id);
    if (fileOpenList.getLinkN(file_id) == 0){
        fileOpenList.deleteItem(file_id);
    }
    // 更新inode
    superBlock.iNodeList_.inode_[inode_id].deleteLink();
    if (superBlock.iNodeList_.inode_[inode_id].getLink() == 0){
        superBlock.iNodeList_.deleteINode(inode_id);
    }
    return true;
}

bool FileSystem::writeFile(std::string filename, std::string content){
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        std::cout << "No such file!" << filename << std::endl;
        return false;
    }
    // 判断是否有权限
    if (!superBlock.iNodeList_.inode_[inode_id].inodeIsAuthor(users.curr_user_)){
        std::cout << "You are not the author!" << std::endl;
        return false;
    }
    // 判断是否为文件
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 0){
        std::cout << "It's a directory!" << std::endl;
        return false;
    }
    // 判断是否打开
    int file_id = userOpenList[users.curr_user_].getFileId(inode_id);
    if (file_id == -1){
        std::cout << "You have not opened this file!" << std::endl;
        return false;
    }
    // 判断是否可以写入
    if (fileOpenList.getMode(file_id) == 0){
        std::cout << "The file is read only!" << std::endl;
        return false;
    }

    // 写入
    int offset = fileOpenList.getOffSet(file_id);
    fileOpenList.setOffSet(file_id, offset + content.size());
    std::string tmp1 = superBlock.iNodeList_.inode_[inode_id].content_.substr(0, offset);
    std::string tmp2 = superBlock.iNodeList_.inode_[inode_id].content_.substr(offset, superBlock.iNodeList_.inode_[inode_id].content_.size() - offset);
    superBlock.iNodeList_.inode_[inode_id].content_ = tmp1 + content + tmp2;

    // 分配磁盘块
    int differ = superBlock.iNodeList_.inode_[inode_id].differ();
    superBlock.iNodeList_.inode_[inode_id].updateFileSize();
    
}

std::string FileSystem::readFile(std::string filename, int len){

}