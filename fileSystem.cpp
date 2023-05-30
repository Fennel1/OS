#include "filesystem.h"

SuperBlock::SuperBlock() {}

std::string getCurrentTime(tm in){
    tm *ct = &in;

	int year, month, day, hour, minute, second;// 年月日时分秒
	year = ct->tm_year + 1900;                 // 年份基础从1900开始的，所以要加上
	month = ct->tm_mon + 1;                    // 月份是0-11，对应1-12月
	day = ct->tm_mday;
	hour = ct->tm_hour;
	minute = ct->tm_min;
	second = ct->tm_sec;
 
	char temp[100];                            // 创建字符数组
	sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d: ", year, month, day, hour, minute, second);// 时间信息合并
	std::string out(temp);                     // 转化为string型
	return move(out);                          // 用move（string）速度快很多
}

int writeLog(std::string msg){
    struct tm *local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	std::string dtime = getCurrentTime(*local);
	std::ofstream outfile;
	outfile.open("..\\debug\\log.txt", std::ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
	if (outfile.is_open()){
		outfile << dtime << msg << "\n";
		outfile.close();
		return 0;
	}
	else{
        return 1;
    }
}


void SuperBlock::createFile(std::string filename, Directory* curr_dir, std::string curr_user){
    // 获取空闲inode
    int inode_id = iNodeList_.getFreeINode();
    if (inode_id == -1){
        //std::cout << "No free inode!" << std::endl;
        std::string err = "createFile: No free inode! " + filename;
        writeLog(err);
        return ;
    }
    // 判断是否有权限
    int pid = curr_dir->getItemId(".");
    if (!iNodeList_.inode_[pid].inodeIsAuthor(curr_user)){
        //std::cout << "You are not the author!" << std::endl;
        std::string err = "createFile: You are not the author! " + filename;
        writeLog(err);
        return ;
    }
    // 判断是否重复
    if (curr_dir->getItemId(filename) != -1){
        //std::cout << "File already exists!" << std::endl;
        std::string err = "createFile: File already exists! " + filename;
        writeLog(err);
        return ;
    }

    // 获取当前时间
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d-%H:%M:%S");
    std::string curr_time = ss.str();

    // 创建inode, 并添加到inode表中
    iNodeList_.addINode(INode(curr_user, 0, 0, 0, 0, curr_time, curr_time, ""), inode_id);
    // 添加到目录中
    curr_dir->addItem(filename, inode_id);
    std::string err = "createFile: Create file successfully! " + filename;
    writeLog(err);
}

void FileSystem::createFile(std::string filename, Directory* curr_dir) {
    // superBlock.createFile(filename, superBlock.iNodeList_.inode_[users.getInodeId()].getDir(), users.curr_user_);
    superBlock.createFile(filename, curr_dir, users.curr_user_);
}

void SuperBlock::deleteFile(std::string filename, Directory* curr_dir, std::string curr_user){
    // 获取inode
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        //std::cout << "No such file!" << std::endl;
        std::string err = "deleteFile: No such file! " + filename;
        writeLog(err);
        return;
    }
    // 判断是否有权限
    if (!iNodeList_.inode_[inode_id].inodeIsAuthor(curr_user)){
        //std::cout << "You are not the author!" << std::endl;
        std::string err = "deleteFile: You are not the author! " + filename;
        writeLog(err);
        return ;
    }
    // 判断是否为文件
    if (iNodeList_.inode_[inode_id].getType() == 1){
        //std::cout << "It's a directory!" << std::endl;
        std::string err = "deleteFile: It's a directory! " + filename;
        writeLog(err);
        return;
    }
    // 删除inode
    if (iNodeList_.deleteINode(inode_id)){
        curr_dir->deleteItem(filename);
        //std::cout << "Delete file successfully!" << std::endl;
        std::string err = "deleteFile: Delete file successfully! " + filename;
        writeLog(err);
    }
    else{
        //std::cout << "Delete file failed!" << std::endl;
        std::string err = "deleteFile: Delete file failed! " + filename;
        writeLog(err);
    }
}

void FileSystem::deleteFile(std::string filename, Directory* curr_dir) {
    superBlock.deleteFile(filename, curr_dir, users.curr_user_);
}

void SuperBlock::createDir(std::string dirname, Directory* curr_dir, std::string curr_user){
    // 获取空闲inode
    int inode_id = iNodeList_.getFreeINode();
    if (inode_id == -1){
        //std::cout << "No free inode!" << std::endl;
        std::string err = "createDir: No free inode! " + dirname;
        writeLog(err);
        return ;
    }
    // 判断是否有权限
    int pid = curr_dir->getItemId(".");
    if (!iNodeList_.inode_[pid].inodeIsAuthor(curr_user)){
        //std::cout << "You are not the author!" << std::endl;
        std::string err = "createDir: You are not the author! " + dirname;
        writeLog(err);
        return ;
    }
    // 判断是否重复
    if (curr_dir->getItemId(dirname) != -1){
        //std::cout << "Directory already exists!" << std::endl;
        std::string err = "createDir: Directory already exists! " + dirname;
        writeLog(err);
        return ;
    }
    // 获取当前时间
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    std::string curr_time = ss.str();

    // 创建inode, 并添加到inode表中
    iNodeList_.addINode(INode(curr_user, 1, 0, 0, 0, curr_time, curr_time, ""), inode_id);
    // 添加到目录中
    curr_dir->addItem(dirname, inode_id);
    iNodeList_.inode_[inode_id].getDir()->init(inode_id, pid);
    std::string err = "createDir: Create directory successfully! " + dirname;
    writeLog(err);
}

void FileSystem::createDir(std::string dirname, Directory* curr_dir) {
    // superBlock.createDir(dirname, superBlock.iNodeList_.inode_[users.getInodeId()].getDir(), users.curr_user_);
    superBlock.createDir(dirname, curr_dir, users.curr_user_);
}

void SuperBlock::deleteDir(std::string dirname, Directory* curr_dir, std::string curr_user){
    // 获取inode
    int inode_id = curr_dir->getItemId(dirname);
    if (inode_id == -1){
        //std::cout << "No such directory!" << std::endl;
        std::string err = "deleteDir: No such directory! " + dirname;
        writeLog(err);
        return;
    }
    // 判断是否有权限
    if (!iNodeList_.inode_[inode_id].inodeIsAuthor(curr_user)){
        //std::cout << "You are not the author!" << std::endl;
        std::string err = "deleteDir: You are not the author! " + dirname;
        writeLog(err);
        return ;
    }
    // 判断是否为目录
    if (iNodeList_.inode_[inode_id].getType() == 0){
        //std::cout << "It's a file!" << std::endl;
        std::string err = "deleteDir: It's a file! " + dirname;
        writeLog(err);
        return;
    }
    // 判断目录是否为空
    if (iNodeList_.inode_[inode_id].dir_.getSize() > 2){
        //std::cout << "Directory is not empty!" << std::endl;
        std::string err = "deleteDir: Directory is not empty! " + dirname;
        writeLog(err);
        return;
    }
    // 删除inode
    if (iNodeList_.deleteINode(inode_id)){
        curr_dir->deleteItem(dirname);
        //std::cout << "Delete directory successfully!" << std::endl;
        std::string err = "deleteDir: Delete directory successfully! " + dirname;
        writeLog(err);
    }
    else{
        //std::cout << "Delete directory failed!" << std::endl;
        std::string err = "deleteDir: Delete directory failed! " + dirname;
        writeLog(err);
    }
}

void FileSystem::deleteDir(std::string dirname, Directory* curr_dir) { 
    superBlock.deleteDir(dirname, curr_dir, users.curr_user_);
}

FileSystem::FileSystem() {
    // 初始化
    superGroup.init();
    users.createUser("root", "123");
    users.login("root", "123");
    int inode_id = superBlock.iNodeList_.getFreeINode();
    superBlock.iNodeList_.addINode(INode("root", 1, 0, 0, 0, "2023-5-28-15:47:12", "2023-5-28-15:47:12", ""), inode_id);
    users.setInodeId(inode_id);
    superBlock.iNodeList_.inode_[inode_id].getDir()->init(inode_id, inode_id);
}

void FileSystem::cd(std::string dirname) {
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(dirname);
    if (inode_id == -1){
        //std::cout << "No such directory! " << dirname << std::endl;
        std::string error="cd: No such directory! " + dirname;
        writeLog(error);
        return;
    }
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 0){
        //std::cout << "It's a file! " << dirname << std::endl;
        std::string error="cd: It's a file! " + dirname;
        writeLog(error);
        return;
    }
    users.setInodeId(inode_id);
    std::string err = "cd: Change directory successfully! " + dirname;
    writeLog(err);
}

bool FileSystem::openFile(std::string filename, int mode, int sign, Directory* curr_dir) {
    // Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        //std::cout << "No such file!" << filename << std::endl;
        std::string error="openFile: No such file! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否有权限
    if (!superBlock.iNodeList_.inode_[inode_id].inodeIsAuthor(users.curr_user_)){
        //std::cout << "You are not the author!" << std::endl;
        std::string error="openFile: You are not the author! " + filename;
        writeLog(error);
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
            //std::cout << "Open file successfully!" << std::endl;
            std::string error="openFile: Open file successfully! " + filename;
            writeLog(error);
            return true;
        }
        else{
            //std::cout << "Open file failed!" << std::endl;
            std::string error="openFile: Open file failed! " + filename;
            writeLog(error);
            return false;
        }
    }
}

bool FileSystem::closeFile(std::string filename, Directory* curr_dir){
    // Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        //std::cout << "No such file!" << filename << std::endl;
        std::string error="closeFile: No such directory! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否有权限
    if (!superBlock.iNodeList_.inode_[inode_id].inodeIsAuthor(users.curr_user_)){
        //std::cout << "You are not the author!" << std::endl;
        std::string error="closeFile: You are not the author! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否为文件
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 1){
        //std::cout << "It's a directory!" << std::endl;
        std::string error="closeFile: It's a directory! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否打开
    if (userOpenList.at(users.curr_user_).findId(inode_id) == -1){
        //std::cout << "File is not open!" << std::endl;
        std::string error="closeFile: File is not open! " + filename;
        writeLog(error);
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
    // if (superBlock.iNodeList_.inode_[inode_id].getLink() == 0){
    //     superBlock.iNodeList_.deleteINode(inode_id);
    // }
    std::string err = "closeFile: Close file successfully! " + filename;
    writeLog(err);
    return true;
}

bool FileSystem::writeFile(std::string filename, std::string content){
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        //std::cout << "No such file!" << filename << std::endl;
        std::string error="writeFile: No such file! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否有权限
    if (!superBlock.iNodeList_.inode_[inode_id].inodeIsAuthor(users.curr_user_)){
        //std::cout << "You are not the author!" << std::endl;
        std::string error="writeFile: You are not the author! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否为文件
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 1){
        //std::cout << "It's a directory!" << std::endl;
        std::string error="writeFile: It's a directory! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否打开
    int file_id = userOpenList[users.curr_user_].getFileId(inode_id);
    if (file_id == -1){
        //std::cout << "You have not opened this file!" << std::endl;
        std::string error="writeFile: You have not opened this file! " + filename;
        writeLog(error);
        return false;
    }
    // 判断是否可以写入
    if (fileOpenList.getMode(file_id) == 0){
        //std::cout << "The file is read only!" << std::endl;
        std::string error="writeFile: The file is read only! " + filename;
        writeLog(error);
        return false;
    }

    // 写入
    if (fileOpenList.getSign(file_id) == 0){    // 覆盖写入
        superBlock.iNodeList_.inode_[inode_id].content_ = content;
        fileOpenList.setSign(file_id, 1);
        fileOpenList.setOffSet(file_id, content.size());
    }
    else{   // 追加写入
        int offset = fileOpenList.getOffSet(file_id);
        fileOpenList.setOffSet(file_id, offset + content.size());
        std::string tmp1 = superBlock.iNodeList_.inode_[inode_id].content_.substr(0, offset);
        std::string tmp2 = superBlock.iNodeList_.inode_[inode_id].content_.substr(offset, superBlock.iNodeList_.inode_[inode_id].content_.size() - offset);
        superBlock.iNodeList_.inode_[inode_id].content_ = tmp1 + content + tmp2;
    }

    // 分配磁盘块
    int differ = superBlock.iNodeList_.inode_[inode_id].differ();
    superBlock.iNodeList_.inode_[inode_id].updateFileSize();
    while(differ > 0){
        int block_id = superGroup.getFreeBlock();
        superBlock.iNodeList_.inode_[inode_id].addBlock(block_id);
        differ--;
    }
    while (differ < 0) {
        int block_id = superBlock.iNodeList_.inode_[inode_id].freeBlock();
        superGroup.addBlockToGroup(block_id);
        differ++;
    }
    std::string err = "writeFile: Write file successfully! " + filename;
    writeLog(err);
    return true;
}

std::string FileSystem::readFile(std::string filename, int len){
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    int inode_id = curr_dir->getItemId(filename);
    if (inode_id == -1){
        //std::cout << "No such file!" << filename << std::endl;
        std::string error="readFile: No such file! " + filename;
        writeLog(error);
        return "";
    }
    // 判断是否有权限
    if (!superBlock.iNodeList_.inode_[inode_id].inodeIsAuthor(users.curr_user_)){
        //std::cout << "You are not the author!" << std::endl;
        std::string error="readFile: You are not the author! " + filename;
        writeLog(error);
        return "";
    }
    // 判断是否为文件
    if (superBlock.iNodeList_.inode_[inode_id].getType() == 1){
        //std::cout << "It's a directory!" << std::endl;
        std::string error="readFile: It's a directory! " + filename;
        writeLog(error);
        return "";
    }
    // 判断是否打开
    int file_id = userOpenList[users.curr_user_].getFileId(inode_id);
    if (file_id == -1){
        //std::cout << "You have not opened this file!" << std::endl;
        std::string error="readFile: You have not opened this file! " + filename;
        writeLog(error);
        return "";
    }
    // 判断是否可以读取
    // if (fileOpenList.getMode(file_id) == 1){
    //     //std::cout << "The file is write only!" << std::endl;
    //     std::string error="readFile: The file is write only! " + filename;
    //     writeLog(error);
    //     return "";
    // }

    // 读取
    std::string res = superBlock.iNodeList_.inode_[inode_id].content_;
    // writeLog(res);
    if (res == ""){
        res = " ";
    }
    std::string err = "readFile: Read file successfully! " + filename;
    writeLog(err);
    return res;
}

void FileSystem::ls() {
    Directory *curr_dir = superBlock.iNodeList_.inode_[users.getInodeId()].getDir();
    std::cout   << std::left << std::setw(8) << "NAME" 
                << std::left << std::setw(8) << "INODE"
                << std::left << std::setw(12) << "USERNAME"
                << std::left << std::setw(8) << "TYPE"
                << std::left << std::setw(8) << "LINK"
                << std::left << std::setw(8) << "SIZE"
                << std::left << std::setw(12) << "BLOCKNUM"
                << std::left << std::setw(25) << "MODTIME"
                << std::left << std::setw(25) << "SETTIME"
                << std::endl;
    for (auto iter = curr_dir->directory.begin(); iter != curr_dir->directory.end(); iter++){
        std::cout   << std::left << std::setw(8) << iter->first ;
        std::cout   << std::left << std::setw(8) << iter->second ;
        std::cout   << std::left << std::setw(12) << superBlock.iNodeList_.inode_[iter->second].getUserName() ;
        std::cout   << std::left << std::setw(8) << superBlock.iNodeList_.inode_[iter->second].getTypeInfo() ;
        std::cout   << std::left << std::setw(8) << superBlock.iNodeList_.inode_[iter->second].getLink() ;
        std::cout   << std::left << std::setw(8) << superBlock.iNodeList_.inode_[iter->second].getSize() ;
        std::cout   << std::left << std::setw(12) << superBlock.iNodeList_.inode_[iter->second].getBlockNum() ;
        std::cout   << std::left << std::setw(25) << superBlock.iNodeList_.inode_[iter->second].getModTime() ;
        std::cout   << std::left << std::setw(25) << superBlock.iNodeList_.inode_[iter->second].getSetTime() << std::endl;
    }
    std::string err = "ls: List directory successfully!";
    writeLog(err);
}

void FileSystem::ls(std::string path) {

}

void FileSystem::rm(std::string path, Directory* curr_dir){

}

void FileSystem::format() {
    for (int i=0; i<INODE_NUM; i++){
        superBlock.iNodeList_.iNodeBitMap_[i] = false;
        superBlock.iNodeList_.inode_[i].clear();
        superBlock.iNodeList_.used_size_ = 0;
    }
    superGroup.init();
    users.userList_.clear();
    users.curr_user_ = "root";
    users.createUser("root", "123");
    userOpenList.clear();
    fileOpenList.clear();
    std::string err = "format: Format successfully!";
    writeLog(err);
}

void FileSystem::save() {
    std::ofstream INodeListout("..\\img\\iNodeList.txt");
    INodeListout << superBlock.iNodeList_.used_size_ << std::endl;
    for (int i=0; i<INODE_NUM; i++){
        if (superBlock.iNodeList_.iNodeBitMap_[i]){
            INodeListout << i << " " << superBlock.iNodeList_.inode_[i].getUserName() << " " 
            << superBlock.iNodeList_.inode_[i].getType() << " " << superBlock.iNodeList_.inode_[i].getLink() << " "
            << superBlock.iNodeList_.inode_[i].getSize() << " " << superBlock.iNodeList_.inode_[i].getBlockNum() << " "
            << superBlock.iNodeList_.inode_[i].getSetTime() << " " << superBlock.iNodeList_.inode_[i].getModTime() << std::endl;
            Directory *curr_dir = superBlock.iNodeList_.inode_[i].getDir();
            INodeListout << curr_dir->directory.size() << " ";
            for (auto iter=curr_dir->directory.begin(); iter!=curr_dir->directory.end(); iter++){
                INodeListout << iter->first << " " << iter->second << " ";
            }
            INodeListout << std::endl;
            std::string file = "..\\img\\content\\" + std::to_string(i) + ".txt";
            std::ofstream contentout(file);
            contentout << superBlock.iNodeList_.inode_[i].content_;
            contentout.close();
        }
    }
    INodeListout.close();

    std::ofstream usersout("..\\img\\users.txt");
    usersout << users.userList_.size()-1 << std::endl;
    for (auto iter=users.userList_.begin(); iter!=users.userList_.end(); iter++){
        if (iter->getUserName() == "root"){
            continue;
        }
        usersout << iter->getUserName() << " " << iter->getPassword() << " " << iter->getInodeId() << std::endl;
    }
    usersout.close();
    std::cout << "Save successfully!" << std::endl;
    std::string err = "save: Save successfully!";
    writeLog(err);
}

void FileSystem::load(){
    format();
    std::ifstream INodeListin("..\\img\\iNodeList.txt");
    int used_size;
    INodeListin >> used_size;
    superBlock.iNodeList_.used_size_ = used_size;
    for (int i=0; i<used_size; i++){
        int inode_id, type, link, size, block_num;
        std::string user_name, set_time, mod_time;
        INodeListin >> inode_id >> user_name >> type >> link >> size >> block_num >> set_time >> mod_time;
        superBlock.iNodeList_.iNodeBitMap_[inode_id] = true;
        superBlock.iNodeList_.inode_[inode_id].username_ = user_name;
        superBlock.iNodeList_.inode_[inode_id].type_ = type;
        superBlock.iNodeList_.inode_[inode_id].nlink_= link;
        superBlock.iNodeList_.inode_[inode_id].file_size_ = size;
        superBlock.iNodeList_.inode_[inode_id].block_num_ = block_num;
        superBlock.iNodeList_.inode_[inode_id].set_time_ = set_time;
        superBlock.iNodeList_.inode_[inode_id].mod_time_ = mod_time;
        Directory *curr_dir = superBlock.iNodeList_.inode_[inode_id].getDir();
        std::string filename;
        int num;
        INodeListin >> num;
        for (int j=0; j<num; j++){
            INodeListin >> filename >> inode_id;
            curr_dir->directory[filename] = inode_id;
        }
        std::string file = "..\\img\\content\\" + std::to_string(inode_id) + ".txt";
        std::ifstream contentin(file);
        std::string content;
        contentin >> content;
        superBlock.iNodeList_.inode_[inode_id].content_ = content;
        contentin.close();
    }
    INodeListin.close();

    std::ifstream usersin("..\\img\\users.txt");
    std::string user_name, password;
    int inode_id, user_num;
    usersin >> user_num;
    for (int i=0; i<user_num; i++){
        usersin >> user_name >> password >> inode_id;
        users.createUser(user_name, password);
        users.userList_[i].inode_id_ = inode_id;
    }
    usersin.close();

    std::string err = "load: Load successfully!";
    writeLog(err);
}