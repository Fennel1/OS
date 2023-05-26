#include "gcm.h"

GroupLeader::GroupLeader() : next_leader_(nullptr) { }

GroupLeader::GroupLeader(GroupLeader *next_leader) : next_leader_(next_leader) { }

GroupLeader::GroupLeader(const GroupLeader &thx) {
    this->size_ = thx.size_;
    this->next_leader_ = thx.next_leader_;
    this->groups_ = thx.groups_;
}

int GroupLeader::getCapacity() const {
    return capacity_;
}

int GroupLeader::getSize() const {
    return size_;
}

int GroupLeader::getBlockId() {
    if (size_ == 0) {
        return -1;
    }
    size_--;
    int block_id = groups_.top();
    groups_.pop();
    return block_id;
}

bool GroupLeader::addBlock(int block_id) {
    if (size_ == capacity_) {
        return false;
    }
    size_++;
    groups_.push(block_id);
    return true;
}

// GroupLeader* getLeader() {
//     return this;
// }

GroupLeader* GroupLeader::getNextLeader() const {
    return next_leader_;
}

bool GroupLeader::setNextLeader(GroupLeader *next_leader) {
    next_leader_ = next_leader;
    return true;
}

int SuperGroup::getFreeGroup() {
    if (free_groupNum_ == 0) {
        return -1;
    }
    if (curr_group_->getSize() == 0) {
        curr_group_ = curr_group_->getNextLeader();
    }
    free_groupNum_--;
    return curr_group_->getBlockId();
}

bool SuperGroup::addBlock(int block_id) {
    if (curr_group_->getSize() == curr_group_->getCapacity()) {
        GroupLeader *new_leader = new GroupLeader(*curr_group_);
        curr_group_ = new GroupLeader(curr_group_);
    }
    curr_group_->addBlock(block_id);
    free_groupNum_++;
    return true;
}

int SuperGroup::getSize() const {
    return free_groupNum_;
}

void SuperGroup::init() {
    for (int i=0; i<TOTAL_GROUP_SIZE/GROUP_SIZE; i++) {
        GroupLeader *group_leader;
        if (i == 0){
            group_leader = new GroupLeader();
            curr_group_ = group_leader;
        }
        else{
            GroupLeader *new_leader = new GroupLeader();
            group_leader->setNextLeader(new_leader);
            group_leader = new_leader;
        }
        for (int j=GROUP_SIZE; j>=1; j--){
            group_leader->addBlock(i*GROUP_SIZE+j);
        }
    }
}