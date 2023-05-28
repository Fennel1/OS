#include "filesystem.h"

SuperBlock::SuperBlock() {
    superGroup_.init();
}

bool SuperBlock::createFile(std::string filename, Directory* cur_dir, std::string curr_user){

}

void FileSystem::createFile(std::string filename) {

}

void SuperBlock::deleteFile(std::string filename, Directory* cur_dir){

}

void FileSystem::deleteFile(std::string filename) {

}