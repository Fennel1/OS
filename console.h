#include "fileSystem.h"
#include "man.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef FILESYSTEM_CONSOLE_H
#define FILESYSTEM_CONSOLE_H

class Console {
public:
    FileSystem fileSystem_;
    Help help_;
    Window window_;

    void run(); 
    std::vector<std::string> splitPath(std::string path, bool &is_root, bool &is_file);    //分割路径
};

#endif //FILESYSTEM_CONSOLE_H