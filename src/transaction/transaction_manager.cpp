#include "transaction_manager.h"
#include "record/rm_file_handle.h"

std::unordered_map<txn_id_t, Transaction *> TransactionManager::txn_map = {};

/**
 * 事务的开始方法
 * @param txn 事务指针
 * @param log_manager 日志管理器，用于日志lab
 * @return 当前事务指针
 * @tips: 事务的指针可能为空指针
 */
Transaction * TransactionManager::Begin(Transaction *txn, LogManager *log_manager) {
    // Todo:
    // 1. 判断传入事务参数是否为空指针
    // 2. 如果为空指针，创建新事务
    // 3. 把开始事务加入到全局事务表中
    // 4. 返回当前事务指针
    // if(!txn)
    //     txn = new Transaction(next_txn_id_++);
    // txn_map[txn->GetTransactionId()] = txn;
    return txn;
}

/**
 * 事务的提交方法
 * @param txn 事务指针
 * @param log_manager 日志管理器，用于日志lab
 * @param sm_manager 系统管理器，用于commit，后续会删掉
 */
void TransactionManager::Commit(Transaction * txn, LogManager *log_manager) {
    // Todo:
    // 1. 如果存在未提交的写操作，提交所有的写操作
    // 2. 释放所有锁
    // 3. 释放事务相关资源，eg.锁集
    // 4. 更新事务状态
    // auto write_set = txn->GetWriteSet();
    // while(!write_set->empty()) {
    //     // auto& item = write_set->back();
    //     // auto context = new Context(lock_manager_, log_manager, txn);
    //     // if(item->GetWriteType() == WType::INSERT_TUPLE)
    //     //     sm_manager_->fhs_.at(item->GetTableName()).get()->insert_record(item->GetRecord().data, context);
    //     // else if (item->GetWriteType() == WType::UPDATE_TUPLE)
    //     //     sm_manager_->fhs_.at(item->GetTableName()).get()->update_record(item->GetRid(), item->GetRecord().data, context);
    //     // else if (item->GetWriteType() == WType::DELETE_TUPLE)
    //     //     sm_manager_->fhs_.at(item->GetTableName()).get()->delete_record(item->GetRid(), context);
    //     write_set->pop_back();
    // }
    // //write_set->clear();
    // for(auto lock_data_id: *txn->GetLockSet())
    //     lock_manager_->Unlock(txn, lock_data_id);
    // txn->SetState(TransactionState::COMMITTED);
}

/**
 * 事务的终止方法
 * @param txn 事务指针
 * @param log_manager 日志管理器，用于日志lab
 * @param sm_manager 系统管理器，用于rollback，后续会删掉
 */
void TransactionManager::Abort(Transaction * txn, LogManager *log_manager) {
    // Todo:
    // 1. 回滚所有写操作
    // 2. 释放所有锁
    // 3. 清空事务相关资源，eg.锁集
    // 4. 更新事务状态
    // auto write_set = txn->GetWriteSet();
    // while (!write_set->empty()) {
    //     auto& item = write_set->back();
    //     auto context = new Context(lock_manager_, log_manager, txn);
    //     // if(item->GetWriteType() == WType::INSERT_TUPLE)
    //     //     sm_manager_->fhs_.at(item->GetTableName()).get()->delete_record(item->GetRid(), context);
    //     // else if (item->GetWriteType() == WType::UPDATE_TUPLE)
    //     //     sm_manager_->fhs_.at(item->GetTableName()).get()->update_record(item->GetRid(), item->GetRecord().data, context);
    //     // else if (item->GetWriteType() == WType::DELETE_TUPLE)
    //     //     sm_manager_->fhs_.at(item->GetTableName()).get()->insert_record(item->GetRid(), item->GetRecord().data);
    //     // 更优雅的写法
    //     if (item->GetWriteType() == WType::INSERT_TUPLE)
    //         sm_manager_->rollback_insert(item->GetTableName(), item->GetRid(), context);
    //     else if (item->GetWriteType() == WType::UPDATE_TUPLE)
    //         sm_manager_->rollback_update(item->GetTableName(), item->GetRid(), item->GetRecord(), context);
    //     else if (item->GetWriteType() == WType::DELETE_TUPLE)
    //         sm_manager_->rollback_delete(item->GetTableName(), item->GetRecord(), context);
    //     write_set->pop_back();
    // }
    // //write_set->clear(); //有点多余，后面考虑删掉
    // for(auto lock_data_id: *txn->GetLockSet())
    //     lock_manager_->Unlock(txn, lock_data_id);
    // txn->SetState(TransactionState::ABORTED);
}

/** 以下函数用于日志实验中的checkpoint */
void TransactionManager::BlockAllTransactions() {}

void TransactionManager::ResumeAllTransactions() {}