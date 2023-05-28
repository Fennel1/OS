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

bool NormalIndex::addIndex(int block_id) {
    if (size_ == capacity_ || block_id < 0) {
        return false;
    }
    indexes_.push_back(block_id);
    size_++;
    return true;
}

int NormalIndex::getSize() const {
    return size_;
}

int NormalIndex::getCapacity() const {
    return capacity_;
}

int NormalIndex::dropIndex() {
    if (size_ == 0) {
        return -1;
    }
    int block_id = indexes_[size_-1];
    indexes_.pop_back();
    size_--;
    return block_id;
}

NormalIndex& NormalIndex::getNextIndex(int n) {
    return indexTables_[n];
}

bool NormalIndex::addNextIndex() {
    if (size_ == capacity_) {
        return false;
    }
    indexTables_.push_back(NormalIndex(sign_-1));
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

MixIndex::MixIndex() : oneIndirectIndex(1), twoIndirectIndex(2), threeIndirectIndex(3) { }

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

bool MixIndex::addDirectIndex(int block_id) {
    if (size_ == capacity_ || block_id < 0) {
        return false;
    }
    indexes_.push_back(block_id);
    size_++;
    return true;
}

bool MixIndex::addOneIndirectIndex(int block_id) {
    if (size_ == capacity_ || block_id < 0) {
        return false;
    }
    if (oneIndirectIndex.addIndex(block_id)) {
        size_++;
        return true;
    }
    return false;
}

bool MixIndex::addTwoIndirectIndex(int block_id) {
    if (size_ == capacity_ || block_id < 0) {
        return false;
    }

    NormalIndex &one = twoIndirectIndex.getNextIndex(twoIndirectIndex.getSize() - 1);
    if (one.getSize() == one.getCapacity()) { // 一次间址索引块已满
        twoIndirectIndex.addNextIndex(); // 新建一个一次间址索引块
        one = twoIndirectIndex.getNextIndex(twoIndirectIndex.getSize() - 1);
    }

    one.addIndex(block_id);
    size_++;
    return true;
}

bool MixIndex::addThreeIndirectIndex(int block_id) {
    if (size_ == capacity_ || block_id < 0) {
        return false;
    }

    NormalIndex &two = threeIndirectIndex.getNextIndex(threeIndirectIndex.getSize() - 1);
    NormalIndex &one = two.getNextIndex(two.getSize() - 1);
    if (one.getSize() == one.getCapacity() && two.getSize() == two.getCapacity()) { // 二次间址索引块已满, 一次间址索引块已满
        threeIndirectIndex.addNextIndex();
        two = threeIndirectIndex.getNextIndex(threeIndirectIndex.getSize() - 1);
        two.addNextIndex();
        one = two.getNextIndex(two.getSize() - 1);
    } else if (one.getSize() == one.getCapacity()) {    // 二次间址索引块未满, 一次间址索引块已满
        two.addNextIndex();
        one = two.getNextIndex(two.getSize() - 1);
    }

    one.addIndex(block_id);
    size_++;
    return true;
}

bool MixIndex::addIndex(int block_id) {
    if (size_ == capacity_ || block_id < 0) {
        return false;
    }

    if (size_ < BASIC_IDX) { // 10
        return addDirectIndex(block_id);
    } else if (size_ < BASIC_IDX + IDXT_SIZE) { // 10 + 16 = 26
        return addOneIndirectIndex(block_id);
    } else if (size_ < BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE) { // 10 + 16 + 256 = 282
        return addTwoIndirectIndex(block_id);
    } else if (size_ < BASIC_IDX + IDXT_SIZE + ONE_IDXT_SIZE + TWO_IDXT_SIZE) { // 10 + 16 + 256 + 4096 = 4382
        return addThreeIndirectIndex(block_id);
    }
    return false;
}

int MixIndex::getSize() const {
    return size_;
}

int MixIndex::dropDirectIndex() {
    indexes_.pop_back();
    size_--;
    return true;
}

int MixIndex::dropOneIndirectIndex() {
    int block_id = oneIndirectIndex.dropIndex();
    if (block_id == -1) {
        return -1;
    }
    size_--;
    return block_id;
}

int MixIndex::dropTwoIndirectIndex() {
    NormalIndex &one = twoIndirectIndex.getNextIndex(twoIndirectIndex.getSize() - 1);
    int block_id = one.dropIndex();
    if (block_id == -1) {
        return -1;
    }
    if (one.getSize() == 0) {
        twoIndirectIndex.dropNextIndex(1);
    }
    size_--;
    return block_id;
}

int MixIndex::dropThreeIndirectIndex() {
    NormalIndex &two = threeIndirectIndex.getNextIndex(threeIndirectIndex.getSize() - 1);
    NormalIndex &one = two.getNextIndex(two.getSize() - 1);
    int block_id = one.dropIndex();
    if (block_id == -1) {
        return -1;
    }
    if (one.getSize() == 0) {
        two.dropNextIndex(1);
    }
    if (two.getSize() == 0) {
        threeIndirectIndex.dropNextIndex(1);
    }
    size_--;
    return block_id;
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
