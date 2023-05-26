#include "memory.h"

NormalIndex::NormalIndex(int sign) : sign_(sign) { }

NormalIndex::NormalIndex(const NormalIndex& thx) {
    sign_ = thx.sign_;
    size_ = thx.size_;
    indexes_ = thx.indexes_;
    indexTables_ = thx.indexTables_;
}

NormalIndex& NormalIndex::operator=(const NormalIndex& thx) {
    sign_ = thx.sign_;
    size_ = thx.size_;
    indexes_ = thx.indexes_;
    indexTables_ = thx.indexTables_;
    return *this;
}

int NormalIndex::getIndex(int n) {
    if (n < 0 || n >= size_) {
        return -1;
    }
    return indexes_[n];
}

bool NormalIndex::addIndex(int id) {
    if (size_ == capacity_ || id < 0) {
        return false;
    }
    indexes_.push_back(id);
    size_++;
    return true;
}

int NormalIndex::getSize() const {
    return size_;
}

bool NormalIndex::dropIndex(int n) {
    if (n <= 0 || n > size_) {
        return false;
    }
    for (int i=0; i<n; i++) {
        indexes_.pop_back();
    }
    size_ -= n;
    return true;
}

int NormalIndex::dropIndex() {
    if (size_ == 0) {
        return -1;
    }
    int id = indexes_.back();
    indexes_.pop_back();
    size_--;
    return id;
}

NormalIndex& NormalIndex::getNextIndex(int n) {
    return indexTables_[n];
}

bool NormalIndex::addNextIndex() {
    if (size_ == capacity_) {
        return false;
    }
    NormalIndex index(sign_-1);
    indexTables_.push_back(index);
    size_++;
    return true;
}

bool NormalIndex::dropNextIndex(int n) {
    if (n <= 0 || n > size_) {
        return false;
    }
    for (int i=0; i<n; i++) {
        indexTables_.pop_back();
    }
    size_ -= n;
    return true;
}

bool NormalIndex::dropNextIndex() {
    if (size_ == 0) {
        return false;
    }
    indexTables_.pop_back();
    size_--;
    return true;
}

int NormalIndex::getSign() const {
    return sign_;
}

void NormalIndex::clear() {
    size_ = 0;
    indexes_.clear();
    indexTables_.clear();
}

std::vector<int> NormalIndex::getIndexes() const {
    return indexes_;
}

MixIndex::MixIndex() : oneIndirectIndex(0), twoIndirectIndex(1), threeIndirectIndex(2) { }

MixIndex::MixIndex(const MixIndex& thx) {
    size_ = thx.size_;
    indexes_ = thx.indexes_;
    oneIndirectIndex = thx.oneIndirectIndex;
    twoIndirectIndex = thx.twoIndirectIndex;
    threeIndirectIndex = thx.threeIndirectIndex;
}

MixIndex& MixIndex::operator=(const MixIndex& thx) {
    size_ = thx.size_;
    indexes_ = thx.indexes_;
    oneIndirectIndex = thx.oneIndirectIndex;
    twoIndirectIndex = thx.twoIndirectIndex;
    threeIndirectIndex = thx.threeIndirectIndex;
    return *this;
}

bool MixIndex::addDirectIndex(int id) {
    if (size_ == capacity_ || id < 0) {
        return false;
    }
    indexes_.push_back(id);
    size_++;
    return true;
}

bool MixIndex::addOneIndirectIndex(int id) {
    if (size_ == capacity_ || id < 0) {
        return false;
    }
    if (oneIndirectIndex.addIndex(id)) {
        size_++;
        return true;
    }
    return false;
}

bool MixIndex::addTwoIndirectIndex(int id) {
    if (size_ == capacity_ || id < 0) {
        return false;
    }

    // 计算超出前两部分索引的数量
    int n = size_ - BASIC_IDX - IDXT_SIZE;
    if(n % IDXT_SIZE == 0){ //正好填满前一个二次间址索引块
        twoIndirectIndex.addNextIndex(); // 新建一个索引块
    }

    //获得当前未满的二次间址索引块
    NormalIndex &t = twoIndirectIndex.getNextIndex(n / IDXT_SIZE);
    t.addIndex(id);
    size_++;
    return true;
}

bool MixIndex::addThreeIndirectIndex(int id) {
    // 计算超出前三部分索引的数量
    int n = size_ - BASIC_IDX - IDXT_SIZE - ONE_IDXT_SIZE;
    if(n % ONE_IDXT_SIZE == 0){ // 正好填满前一个二次间址索引块
        threeIndirectIndex.addNextIndex(); // 新建一个二次间址索引块
    }

    // 获得当前未满的二次间址索引块
    NormalIndex &t = threeIndirectIndex.getNextIndex(n / ONE_IDXT_SIZE);
    if(n % IDXT_SIZE == 0){ // 正好填满一个三次间址索引块
        t.addNextIndex(); // 新建一个三次间址索引块
    }

    // 获得当前未满的三次间址索引块
    NormalIndex &nxt = t.getNextIndex((n % ONE_IDXT_SIZE) / IDXT_SIZE);
    nxt.addIndex(id);
    size_++;
    return true;
}

bool MixIndex::addIndex(int id) {
    if (size_ == capacity_ || id < 0) {
        return false;
    }
    if (id < BASIC_IDX) {
        return addDirectIndex(id);
    } else if (id < BASIC_IDX + IDXT_SIZE) {
        return addOneIndirectIndex(id);
    } else if (id < BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE) {
        return addTwoIndirectIndex(id);
    } else if (id < BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE + TWO_IDXT_SIZE) {
        return addThreeIndirectIndex(id);
    }
    return false;
}

int MixIndex::getSize() const {
    return size_;
}

int MixIndex::dropDirectIndex() {
    int id = indexes_.back();
    indexes_.pop_back();
    size_--;
    return id;
}

int MixIndex::dropOneIndirectIndex() {
    int id = oneIndirectIndex.dropIndex();
    size_--;
    return id;
}

int MixIndex::dropTwoIndirectIndex() {
    int n = size_ - BASIC_IDX - IDXT_SIZE - 1;
    int id = twoIndirectIndex.getNextIndex(n / IDXT_SIZE).dropIndex();
    size_--;
    if ((n+1) % IDXT_SIZE == 1) {
        twoIndirectIndex.dropNextIndex();
    }
    return id;
}

int MixIndex::dropThreeIndirectIndex() {
    int n = size_ - BASIC_IDX - IDXT_SIZE - ONE_IDXT_SIZE - 1;
    int id = threeIndirectIndex.getNextIndex(n / ONE_IDXT_SIZE).getNextIndex((n % ONE_IDXT_SIZE) / IDXT_SIZE).dropIndex();
    size_--;
    if (((n+1) % ONE_IDXT_SIZE) % IDXT_SIZE == 1) {
        threeIndirectIndex.getNextIndex(n / ONE_IDXT_SIZE).dropNextIndex();
    }
    if ((n+1) % ONE_IDXT_SIZE == 1) {
        threeIndirectIndex.dropNextIndex();
    }
    return id;
}

int MixIndex::dropIndex() {
    if (size_ == 0) {
        return -1;
    }
    if (size_ <= BASIC_IDX) {
        return dropDirectIndex();
    } else if (size_ <= BASIC_IDX + IDXT_SIZE) {
        return dropOneIndirectIndex();
    } else if (size_ <= BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE) {
        return dropTwoIndirectIndex();
    } else if (size_ <= BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE + TWO_IDXT_SIZE) {
        return dropThreeIndirectIndex();
    }
    return -1;
}

void MixIndex::clear() {
    size_ = 0;
    indexes_.clear();
    oneIndirectIndex.clear();
    twoIndirectIndex.clear();
    threeIndirectIndex.clear();
}
std::vector<int> MixIndex::getIndexes() {
    std::vector<int> res;
    for (int i = 0; i < size_; i++) {
        res.push_back(indexes_[i]);
    }
    if (size_ > BASIC_IDX){
        std::vector<int> tmp = oneIndirectIndex.getIndexes();
        for (int i = 0; i < tmp.size(); i++) {
            res.push_back(tmp[i]);
        }
    }
    if (size_ > BASIC_IDX + IDXT_SIZE){
        int n = twoIndirectIndex.getSize();
        for (int i = 0; i < n; i++) {
            std::vector<int> tmp = twoIndirectIndex.getNextIndex(i).getIndexes();
            for (int j = 0; j < tmp.size(); j++) {
                res.push_back(tmp[j]);
            }
        }
    }
    if (size_ > BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE){
        int n = threeIndirectIndex.getSize();
        for (int i = 0; i < n; i++) {
            int m = threeIndirectIndex.getNextIndex(i).getSize();
            for (int j = 0; j < m; j++) {
                std::vector<int> tmp = threeIndirectIndex.getNextIndex(i).getNextIndex(j).getIndexes();
                for (int k = 0; k < tmp.size(); k++) {
                    res.push_back(tmp[k]);
                }
            }
        }
    }
    return res;
}
