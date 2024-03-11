#include "rm_scan.h"

#include "rm_file_handle.h"

/**
 * @brief 初始化file_handle和rid
 *
 * @param file_handle
 */
RmScan::RmScan(const RmFileHandle *file_handle) : file_handle_(file_handle) {
    // Todo:
    //初始化file_handle和rid（指向第一个存放了记录的位置）
    int page_no = file_handle->file_hdr_.num_pages; // 当前文件总页数
    if(page_no == 1) { //没有记录
        rid_ = Rid{0, file_handle->file_hdr_.num_records_per_page};
        return;
    }
    RmPageHandle page_handle = file_handle->fetch_page_handle(1); 
    rid_ = Rid({1, Bitmap::first_bit(1, page_handle.bitmap, file_handle->file_hdr_.num_records_per_page)});
}

/**
 * @brief 找到文件中下一个存放了记录的位置
 */
void RmScan::next() {
    // Todo:
    // 找到文件中下一个存放了记录的非空闲位置，用rid_来指向这个位置
    RmPageHandle page_handle = file_handle_->fetch_page_handle(rid_.page_no);
    int next = Bitmap::next_bit(1, page_handle.bitmap, file_handle_->file_hdr_.num_records_per_page, rid_.slot_no);
    if(next == file_handle_->file_hdr_.num_records_per_page) {
        int page_no = file_handle_->file_hdr_.num_pages; // 当前文件总页数
        for(int i = rid_.page_no + 1; i < page_no; ++i) {
            RmPageHandle _page_handle = file_handle_->fetch_page_handle(i);
            int _next = Bitmap::first_bit(i, _page_handle.bitmap, file_handle_->file_hdr_.num_records_per_page);
            if(_next != file_handle_->file_hdr_.num_records_per_page) {
                rid_ = Rid({i, _next});
                return;
            }
        }
        rid_ = Rid({page_no, 0}); 
        return;
    }
    rid_ = Rid({rid_.page_no, next});
}

/**
 * @brief ​ 判断是否到达文件末尾
 */
bool RmScan::is_end() const {
    // Todo: 修改返回值
    if(rid_.page_no >= file_handle_->file_hdr_.num_pages || rid_.slot_no == file_handle_->file_hdr_.num_records_per_page)
        return true;
    return false;
}

/**
 * @brief RmScan内部存放的rid
 */
Rid RmScan::rid() const {
    // Todo: 修改返回值
    return rid_;
}