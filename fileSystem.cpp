#include "filesystem.h"

SuperBlock::SuperBlock() {
    superGroup_.init();
}

bool SuperBlock::createFile(std::string filename, Directory* cur_dir){
}