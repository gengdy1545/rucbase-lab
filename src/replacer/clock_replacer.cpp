#include "replacer/clock_replacer.h"

#include <algorithm>

ClockReplacer::ClockReplacer(size_t num_pages)
    : circular_{num_pages, ClockReplacer::Status::EMPTY_OR_PINNED}, hand_{0}, capacity_{num_pages} {
    // 成员初始化列表语法
    circular_.reserve(num_pages);
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // Todo: try to find a victim frame in buffer pool with clock scheme
    // and make the *frame_id = victim_frame_id
    // not found, frame_id=nullptr and return false
    int max = 2 * capacity_; // max travel times
    while(max--) {
        if(circular_[hand_] == ClockReplacer::Status::UNTOUCHED) {
            circular_[hand_] = ClockReplacer::Status::EMPTY_OR_PINNED;
            *frame_id = ++hand_;
            hand_ %= capacity_;
            return true;
        }
        else if(circular_[hand_] == ClockReplacer::Status::ACCESSED) 
            circular_[hand_] = ClockReplacer::Status::UNTOUCHED;
        hand_ = (hand_ + 1) % capacity_;
    }
    return false;
}

void ClockReplacer::Pin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // Todo: you can implement it!
    if(circular_[frame_id] == ClockReplacer::Status::UNTOUCHED)
        circular_[frame_id] = ClockReplacer::Status::EMPTY_OR_PINNED;
}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // Todo: you can implement it!
    circular_[frame_id] = ClockReplacer::Status::ACCESSED;
}

size_t ClockReplacer::Size() {
    // Todo:
    // 返回在[arg0, arg1)范围内满足特定条件(arg2)的元素的数目
    // return all items that in the range[circular_.begin, circular_.end )
    // and be met the condition: status!=EMPTY_OR_PINNED
    // That is the number of frames in the buffer pool that storage page (NOT EMPTY_OR_PINNED)
    int sum = 0;
    for(size_t i = 0; i < capacity_; ++i) 
        if(circular_[i] != ClockReplacer::Status::EMPTY_OR_PINNED)
            ++sum;
    return sum;
}
