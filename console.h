#include "fileSystem.h"
#include "man.h"
#include "window.h"

#ifndef FILESYSTEM_CONSOLE_H
#define FILESYSTEM_CONSOLE_H

class Console {
public:
    FileSystem fileSystem_;
    Help help_;
    Window window_;

    void run(); 
};

#endif //FILESYSTEM_CONSOLE_H