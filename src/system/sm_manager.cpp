#include "sm_manager.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "index/ix.h"
#include "record/rm.h"
#include "record_printer.h"

bool SmManager::is_dir(const std::string &db_name) {
    struct stat st;
    return stat(db_name.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}
void SmManager::create_db(const std::string &db_name) {
    // lab3 task1 Todo
    // 利用*inx命令创建目录作为数据库
    // lab3 task1 Todo End

    // 根据实验指导
    if(is_dir(db_name))
        throw DatabaseExistsError(db_name);
    // create a subdirectory for the database
    std::string cmd = "mkdir " + db_name;
    if(system(cmd.c_str()) < 0)  //创建一个名为db_name的目录
        throw UnixError();
    if(chdir(db_name.c_str()) < 0) //进入名为db_name的目录
        throw UnixError();
    // create the system catalogs
    DbMeta *new_db = new DbMeta();
    new_db->name_ = db_name;

    // 注意，此时ofstream会在当前目录创建（如果没有此文件先创建）和打开一个名为DB_META_NAME的文件
    std::ofstream ofs(DB_META_NAME);

    // 将new_db中的信息，按照定义好的operator<<操作符，写入到ofs打开的DB_META_NAME文件中
    ofs << *new_db;

    //释放内存中的DBMeta，并回退到上一级目录
    delete new_db;
    if(chdir("..") < 0)
        throw UnixError();
}

void SmManager::drop_db(const std::string &db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    std::string cmd = "rm -r " + db_name;
    if (system(cmd.c_str()) < 0) {
        throw UnixError();
    }
}

void SmManager::open_db(const std::string &db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    // cd to database dir
    if (chdir(db_name.c_str()) < 0) {
        throw UnixError();
    }
    // Load meta
    // 打开一个名为DB_META_NAME的文件
    std::ifstream ifs(DB_META_NAME);
    // 将ofs打开的DB_META_NAME文件中的信息，按照定义好的operator>>操作符，读出到db_中
    ifs >> db_;  // 注意：此处重载了操作符>>
    // Open all record files & index files
    for (auto &entry : db_.tabs_) {
        auto &tab = entry.second;
        // fhs_[tab.name] = rm_manager_->open_file(tab.name);
        fhs_.emplace(tab.name, rm_manager_->open_file(tab.name));
        for (size_t i = 0; i < tab.cols.size(); i++) {
            auto &col = tab.cols[i];
            if (col.index) {
                auto index_name = ix_manager_->get_index_name(tab.name, i);
                assert(ihs_.count(index_name) == 0);
                // ihs_[index_name] = ix_manager_->open_index(tab.name, i);
                ihs_.emplace(index_name, ix_manager_->open_index(tab.name, i));
            }
        }
    }
}

void SmManager::close_db() {
    // lab3 task1 Todo
    // 清理db_
    // 关闭rm_manager_ ix_manager_文件
    // 清理fhs_, ihs_
    // lab3 task1 Todo End

    //  根据实验指导
    // 利用获取的DBMeta成员变量将meta数据Dump到文件中
    std::ofstream ofs(DB_META_NAME);
    ofs << db_;
    // 清理 SmManager.db_，让系统知道现在的db_已经重置了
    db_.name_.clear();
    db_.tabs_.clear();

    // 关闭数据库的记录文件和索引文件
    for(auto &entry: fhs_) 
        rm_manager_->close_file(entry.second.get());
    fhs_.clear();
    for(auto &entry: ihs_)
        ix_manager_->close_index(entry.second.get());
    ihs_.clear();

    // 回退到上一级目录
    if(chdir("..") < 0)
        throw UnixError();       
}

void SmManager::show_tables(Context *context) {
    RecordPrinter printer(1);
    printer.print_separator(context);
    printer.print_record({"Tables"}, context);
    printer.print_separator(context);
    for (auto &entry : db_.tabs_) {
        auto &tab = entry.second;
        printer.print_record({tab.name}, context);
    }
    printer.print_separator(context);
}

void SmManager::desc_table(const std::string &tab_name, Context *context) {
    TabMeta &tab = db_.get_table(tab_name);

    std::vector<std::string> captions = {"Field", "Type", "Index"};
    RecordPrinter printer(captions.size());
    // Print header
    printer.print_separator(context);
    printer.print_record(captions, context);
    printer.print_separator(context);
    // Print fields
    for (auto &col : tab.cols) {
        std::vector<std::string> field_info = {col.name, coltype2str(col.type), col.index ? "YES" : "NO"};
        printer.print_record(field_info, context);
    }
    // Print footer
    printer.print_separator(context);
}

void SmManager::create_table(const std::string &tab_name, const std::vector<ColDef> &col_defs, Context *context) {
    if (db_.is_table(tab_name)) {
        throw TableExistsError(tab_name);
    }
    // Create table meta
    int curr_offset = 0;
    TabMeta tab;
    tab.name = tab_name;
    for (auto &col_def : col_defs) {
        ColMeta col = {.tab_name = tab_name,
                       .name = col_def.name,
                       .type = col_def.type,
                       .len = col_def.len,
                       .offset = curr_offset,
                       .index = false};
        curr_offset += col_def.len;
        tab.cols.push_back(col);
    }
    // Create & open record file
    int record_size = curr_offset;  // record_size就是col meta所占的大小（表的元数据也是以记录的形式进行存储的）
    rm_manager_->create_file(tab_name, record_size);
    db_.tabs_[tab_name] = tab;
    // fhs_[tab_name] = rm_manager_->open_file(tab_name);
    fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));
}

void SmManager::drop_table(const std::string &tab_name, Context *context) {
    // lab3 task1 Todo
    // Find table index in db_ meta
    // Close & destroy record file
    // Close & destroy index file
    // lab3 task1 Todo End

    // 根据实验指导
    // 获取表元数据
    TabMeta &tab = db_.get_table(tab_name);

    // 删除记录文件
    rm_manager_->close_file(fhs_[tab_name].get());
    rm_manager_->destroy_file(tab_name); // 根据creat_table，创建时文件名就为表名

    // 删除索引文件
    for(auto &col: tab.cols)
        if(col.index)
            drop_index(tab_name, col.name, context);

    // 在数据库元数据文件和记录文件句柄中删除该表信息
    db_.tabs_.erase(tab_name);
    fhs_.erase(tab_name);

    // 标记当前表已被删除
    tab.delete_mark_ = true;
}

void SmManager::create_index(const std::string &tab_name, const std::string &col_name, Context *context) {
    TabMeta &tab = db_.get_table(tab_name);
    auto col = tab.get_col(col_name);
    if (col->index) {
        throw IndexExistsError(tab_name, col_name);
    }
    // Create index file
    int col_idx = col - tab.cols.begin();
    ix_manager_->create_index(tab_name, col_idx, col->type, col->len);  // 这里调用了
    // Open index file
    auto ih = ix_manager_->open_index(tab_name, col_idx);
    // Get record file handle
    auto file_handle = fhs_.at(tab_name).get();
    // Index all records into index
    for (RmScan rm_scan(file_handle); !rm_scan.is_end(); rm_scan.next()) {
        auto rec = file_handle->get_record(rm_scan.rid(), context);  // rid是record的存储位置，作为value插入到索引里
        const char *key = rec->data + col->offset;
        // record data里以各个属性的offset进行分隔，属性的长度为col len，record里面每个属性的数据作为key插入索引里
        ih->insert_entry(key, rm_scan.rid(), context->txn_);
    }
    // Store index handle
    auto index_name = ix_manager_->get_index_name(tab_name, col_idx);
    assert(ihs_.count(index_name) == 0);
    // ihs_[index_name] = std::move(ih);
    ihs_.emplace(index_name, std::move(ih));
    // Mark column index as created
    col->index = true;
}

void SmManager::drop_index(const std::string &tab_name, const std::string &col_name, Context *context) {
    TabMeta &tab = db_.tabs_[tab_name];
    auto col = tab.get_col(col_name);
    if (!col->index) {
        throw IndexNotFoundError(tab_name, col_name);
    }
    int col_idx = col - tab.cols.begin();
    auto index_name = ix_manager_->get_index_name(tab_name, col_idx);
    ix_manager_->close_index(ihs_.at(index_name).get());
    ix_manager_->destroy_index(tab_name, col_idx);
    ihs_.erase(index_name);
    col->index = false;
}

// Transaction rollback management
/**
 * @brief rollback the insert operation
 *
 * @param tab_name the name of the table
 * @param rid the rid of the record
 * @param txn the transaction
 */
void SmManager::rollback_insert(const std::string &tab_name, const Rid &rid, Context *context)
{
    // fhs_.at(tab_name).get()->delete_record(rid, context);
    auto file_handler = fhs_.at(tab_name).get();
    file_handler->delete_record(rid, context);
}

/**
 * @brief rollback the delete operation
 *
 * @param tab_name the name of the table
 * @param rid the value of the deleted record
 * @param txn the transaction
 */
void SmManager::rollback_delete(const std::string &tab_name, const RmRecord &record, Context *context) 
{
    // fhs_.at(tab_name).get()->insert_record(record.data, context);
    auto file_handler = fhs_.at(tab_name).get();
    file_handler->insert_record(record.data, context);
    printf("rollback_delete: %s\n", record.data);
}

/**
 * @brief rollback the update operation
 *
 * @param tab_name the name of the table
 * @param rid the rid of the record
 * @param record the old value of the record
 * @param txn the transaction
 */
void SmManager::rollback_update(const std::string &tab_name, const Rid &rid, const RmRecord &record, Context *context)
{
    // fhs_.at(tab_name).get()->update_record(rid, record.data, context);
    auto file_handler = fhs_.at(tab_name).get();
    file_handler->update_record(rid, record.data, context);
    printf("rollback_updata: %s\n", record.data);
}