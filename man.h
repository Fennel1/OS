#include <string>
#include <iostream>

#ifndef MAN_H
#define MAN_H

class Help {
private:

public:
    void help();
    void man(std::string command);
};

#endif // MAN_H
