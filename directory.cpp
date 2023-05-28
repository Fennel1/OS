#include "directory.h"

Directory::Directory(const Directory& thx) {
    directory = thx.directory;
}

Directory& Directory::operator=(const Directory& thx) {
    directory = thx.directory;
    return *this;
}

bool Directory::addItem(std::string filename, int id) {
    if (directory.find(filename) != directory.end()) {
        return false;
    }
    directory[filename] = id;
    return true;
}

bool Directory::deleteItem(std::string filename) {
    if (directory.find(filename) == directory.end()) {
        return false;
    }
    directory.erase(filename);
    return true;
}

bool Directory::checkItem(std::string filename) {
    return directory.find(filename) != directory.end();
}

int Directory::getItemId(std::string filename) {
    if (directory.find(filename) == directory.end()) {
        return -1;
    }
    return directory[filename];
}

bool Directory::setFileName(std::string filename, std::string newname) {
    if (directory.find(filename) == directory.end() || directory.find(newname) != directory.end()) {
        return false;
    }
    directory[newname] = directory[filename];
    directory.erase(filename);
    return true;
}

int Directory::getSize() {
    return directory.size();
}

void Directory::init(int id, int parent_id) {
    directory.clear();
    directory["."] = id;
    directory[".."] = parent_id;
}

void Directory::clear() {
    directory.clear();
}

std::string Directory::getFileName(int id) {
    for (auto it = directory.begin(); it != directory.end(); it++) {
        if (it->second == id) {
            return it->first;
        }
    }
    return "";
}

void Directory::show() {
    for (auto it = directory.begin(); it != directory.end(); it++) {
        std::cout << it->first << " " << it->second << std::endl;
    }
}