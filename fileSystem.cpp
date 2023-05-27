#include "filesystem.h"

SuperBlock::SuperBlock() {
    superGroup_.init();
}

bool SuperBlock::createFile(std::string filename, Directory* cur_dir, std::string curr_user){
    int i = iNodeList_.getFreeINode();
    if (i == -1){
        std::cout << "No free iNode!" << std::endl;
        return false;
    }
    int id = cur_dir->getItemId(".");
    if (!iNodeList_.getINode(id).inodeIsAuthor(curr_user)){
        std::cout << "you are not authenticated!"<< std::endl;
        return false;
    }

    cur_dir->addItem(filename, i);
    iNodeBitMap[i] = true;
    int differ = iNodeList_.getINode(id).differ();
    iNodeList_.getINode(id).updateFileSize();
    while(differ > 0){
        int bid = superGroup_.getFreeGroup();
        if (bid == -1){
            std::cout << "No free block!" << std::endl;
            return false;
        }
        iNodeList_.getINode(id).addBlock(bid);
        differ--;
    }
    while (differ < 0){
        int bid = iNodeList_.getINode(id).freeBlock();
        superGroup_.addBlock(bid);
        differ++;
    }
    if (iNodeList_.addINode(INode(curr_user, 0, 0, 0, 0, "", "", ""), i)){
        std::cout << "Create file success!" << std::endl;
        free_inodeNum_--;
        return true;
    } else {
        std::cout << "Create file failed!" << std::endl;
        return false;
    }
}

void FileSystem::createFile(std::string filename) {
    Directory *cur_dir = users.getCurDir();
    if (cur_dir == nullptr) {
        std::cout << "Please login first!" << std::endl;
        return;
    }

    if (cur_dir->checkItem(filename)) {
        std::cout << "File already exists! : " << filename << std::endl;
        return;
    }

    if (superBlock.createFile(filename, cur_dir, users.curr_user_)) {
        
    } else {
        std::cout << "you are not authenticated!"<< std::endl;
    }
}

void SuperBlock::deleteFile(std::string filename, Directory* cur_dir){
    int pos = cur_dir->getItemId(filename);
    if (pos == -1){
        std::cout << "No such file! :" << filename << std::endl;
        return;
    }
    cur_dir->deleteItem(filename);
    iNodeBitMap[pos] = false;
    iNodeList_.deleteINode(pos);
    free_inodeNum_++;
}

void FileSystem::deleteFile(std::string filename) {
    Directory *cur_dir = users.getCurDir();
    if (cur_dir == nullptr) {
        std::cout << "Please login first!" << std::endl;
        return;
    }
    int pos = cur_dir->getItemId(filename);
    if (pos == -1){
        std::cout << "No such file! :" << filename << std::endl;
        return;
    }

    if (superBlock.iNodeList_.getINode(pos).inodeIsAuthor(users.curr_user_)){
        superBlock.deleteFile(filename, cur_dir);
        std::cout << "Delete file success!" << std::endl;
    } else {
        std::cout << "you are not authenticated!"<< std::endl;
    }
}