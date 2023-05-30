#include "man.h"

void Help::help(){
    std::cout << "Usage of all commands:" << std::endl;
    std::cout << "create   :create <path>" << std::endl;
    std::cout << "delete   :delete <path>" << std::endl;
    std::cout << "open     :open <path> <mode> <sign>" << std::endl;
    std::cout << "close    :close <path>" << std::endl;
    std::cout << "read     :read <path> <len>" << std::endl;
    std::cout << "write    :write <path>" << std::endl;
    std::cout << "cd       :cd <path>" << std::endl;
    std::cout << "ls       :ls or ls <path>" << std::endl;
    std::cout << "rm       :rm <path>" << std::endl;
    std::cout << "login    :login <username> <password>" << std::endl;
    std::cout << "logout   :logout" << std::endl;
    std::cout << "adduser  :adduser <username> <password>" << std::endl;
    std::cout << "switch   :switch <username>" << std::endl;
    std::cout << "format   :format" << std::endl;
    std::cout << "save     :save" << std::endl;
    std::cout << "load     :load " << std::endl;
    std::cout << "rename   :rename <oldname> <newname>" << std::endl;
    std::cout << "help     :help" << std::endl;
}

void Help::man(std::string command){
    if (command == "create"){
        std::cout << "Usage: create <path>" << std::endl;
        std::cout << "Create a new file or catalog" << std::endl;
    }
    else if(command == "delete"){
        std::cout << "Usage: delete <path>" << std::endl;
        std::cout << "Delete a existed file or catalog" << std::endl;
    }
    else if(command == "open"){
        std::cout << "Usage: open <path> <mode> <sign>" << std::endl;
        std::cout << "Open a file" << std::endl;
    }
    else if(command == "close"){
        std::cout << "Usage: close <path>" << std::endl;
        std::cout << "Close a file" << std::endl;
    }
    else if(command == "read"){
        std::cout << "Usage: read <path> <len>" << std::endl;
        std::cout << "Read a file and display messages to window" << std::endl;
    }
    else if(command == "write"){
        std::cout << "Usage: write <path>" << std::endl;
        std::cout << "Write messages from window to a file" << std::endl;
    }
    else if(command == "cd"){
        //切换目录
        std::cout << "Usage: cd <path>" << std::endl;
        std::cout << "Switch to another catalog" << std::endl;
    }
    else if(command == "ls"){
        //显示当前目录下的内容
        std::cout << "Usage: ls or ls <path>" << std::endl;
        std::cout << "Display the content in the current catalog" << std::endl;
    }
    else if(command == "rm"){
        std::cout << "Usage: rm <path>" << std::endl;
        std::cout << "Recursive delete catalogs" << std::endl;
    }
    else if(command == "login"){
        std::cout << "Usage: login <username> <password>" << std::endl;
        std::cout << "Login an account" << std::endl;
    }
    else if(command == "logout"){
        std::cout << "Usage: logout" << std::endl;
        std::cout << "Logout an account" << std::endl;
    }
    else if(command == "adduser"){
        std::cout << "Usage: adduser <username> <password>" << std::endl;
        std::cout << "Add new account to users" << std::endl;
    }
    else if(command == "switch"){
        std::cout << "Usage: switch <username>" << std::endl;
        std::cout << "Users logined switch to another account" << std::endl;
    }
    else if(command == "format"){//格式化文件系统
        std::cout << "Usage: format" << std::endl;
        std::cout << "Format the file system" << std::endl;
    }
    else if(command == "save"){
        std::cout << "Usage: save" << std::endl;
        std::cout << "Save the file system" << std::endl;
    }
    else if(command == "load"){
        std::cout << "Usage: load" << std::endl;
        std::cout << "Load the file system" << std::endl;
    }
    else if(command == "rename"){
        std::cout << "Usage: rename <oldname> <newname>" << std::endl;
        std::cout << "Give a new name to a file or catalog" << std::endl;
    }
    else if(command == "help"){
        std::cout << "Usage: help" << std::endl;
        std::cout << "Check the HELP message" << std::endl;
    }
}