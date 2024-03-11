#include "lock_manager.h"

/**
 * 申请行级读锁
 * @param txn 要申请锁的事务对象指针
 * @param rid 加锁的目标记录ID
 * @param tab_fd 记录所在的表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockSharedOnRecord(Transaction *txn, const Rid &rid, int tab_fd) {
    return true;
    // // Todo:
    // // 1. 通过mutex申请访问全局锁表
    // // 2. 检查事务的状态
    // // 3. 查找当前事务是否已经申请了目标数据项上的锁，如果存在则根据锁类型进行操作，否则执行下一步操作
    // // 4. 将要申请的锁放入到全局锁表中，并通过组模式来判断是否可以成功授予锁
    // // 5. 如果成功，更新目标数据项在全局锁表中的信息，否则阻塞当前操作
    // // 提示：步骤5中的阻塞操作可以通过条件变量来完成，所有加锁操作都遵循上述步骤，在下面的加锁操作中不再进行注释提示
    // std::unique_lock<std::mutex> latch{latch_};
    // if(txn->GetState() == TransactionState::ABORTED || txn->GetState() == TransactionState::COMMITTED)
    //     return false;
    // /* 
    // * REPEATABLE_READ:
    // * The transaction is required to take all locks.
    // * All locks are allowed in the GROWING state
    // * No locks are allowed in the SHRINKING state

    // * READ_COMMITTED:
    // * The transaction is required to take all locks.
    // * All locks are allowed in the GROWING state
    // * Only IS, S locks are allowed in the SHRINKING state

    // * READ_UNCOMMITTED:
    // * The transaction is required to take only IX, X locks.
    // * X, IX locks are allowed in the GROWING state.
    // * S, IS, SIX locks are never allowed
    // */
    // if(txn->GetIsolationLevel() == IsolationLevel::READ_UNCOMMITTED){
    //     txn->SetState(TransactionState::ABORTED);
    //     // throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_READ_UNCOMMITTED);
    //     assert(false);
    // }
    // if((txn->GetState() == TransactionState::SHRINKING) && (
    //   txn->GetIsolationLevel() == IsolationLevel::SERIALIZABLE || 
    //   txn->GetIsolationLevel() == IsolationLevel::REPEATABLE_READ)) {
    //     txn->SetState(TransactionState::ABORTED);
    //     throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
    // }
    // txn->SetState(TransactionState::GROWING);
    // LockDataId lock_id(tab_fd, rid, LockDataType::RECORD);
    // if (txn->GetLockSet()->count(lock_id) == 0) { // if not exist
    //     txn->GetLockSet()->insert(lock_id); // add into lock set
    //     auto lock_request = new LockRequest(txn->GetTransactionId(), LockMode::SHARED);
    //     lock_table_[lock_id].request_queue_.push_back(*lock_request);
    //     lock_table_[lock_id].shared_lock_num_++;
    //     // S is not compatible with X/IX/SIX
    //     while (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::X ||
    //              lock_table_[lock_id].group_lock_mode_ == GroupLockMode::IX ||
    //              lock_table_[lock_id].group_lock_mode_ == GroupLockMode::SIX) {
    //         lock_table_[lock_id].cv_.wait(latch);
    //     }
    //     lock_table_[lock_id].group_lock_mode_ = GroupLockMode::S; // S/IS/NON_LOCK -> S
    //     lock_request->granted_ = true;
    //     lock_table_[lock_id].cv_.notify_all();
    //     return true;
    // }
    // // if exist (only earse one element, so don't consider iterator bug)
    // for (auto request = lock_table_[lock_id].request_queue_.begin(); request != lock_table_[lock_id].request_queue_.end(); request++) {
    //     if (request->txn_id_ == txn->GetTransactionId()) {
    //         if (request->lock_mode_ == LockMode::SHARED) // if the same as before, return directly
    //             return true;
    //         if(lock_table_[lock_id].upgrading_) { // if upgrading, abort and throw
    //             txn->SetState(TransactionState::ABORTED);
    //             throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //         }
    //         // While upgrading, only the following transitions should be allowed: 
    //         // IS -> [S, X, IX, SIX] S -> [X, SIX] IX -> [X, SIX] SIX -> [X]
    //         if(request->lock_mode_ == LockMode::INTENTION_SHARED) {
    //             lock_table_[lock_id].upgrading_ = true;
    //             lock_table_[lock_id].request_queue_.erase(request);
    //             lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::SHARED));
    //             lock_table_[lock_id].group_lock_mode_ = GroupLockMode::S;
    //             return true;
    //         } else if (request->lock_mode_ == LockMode::INTENTION_EXCLUSIVE) {
    //             lock_table_[lock_id].upgrading_ = true;
    //             lock_table_[lock_id].request_queue_.erase(request);
    //             lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::S_IX));
    //             lock_table_[lock_id].group_lock_mode_ = GroupLockMode::SIX;
    //             return true;
    //         }
    //         // else abort and throw
    //         txn->SetState(TransactionState::ABORTED);
    //         throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //     }
    // }
    // assert(false);
}

/**
 * 申请行级写锁
 * @param txn 要申请锁的事务对象指针
 * @param rid 加锁的目标记录ID
 * @param tab_fd 记录所在的表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockExclusiveOnRecord(Transaction *txn, const Rid &rid, int tab_fd) {
    return true;
    // std::unique_lock<std::mutex> latch{latch_};
    // if(txn->GetState() == TransactionState::ABORTED || txn->GetState() == TransactionState::COMMITTED)
    //     return false;
    // if((txn->GetState() == TransactionState::SHRINKING) && (
    //   txn->GetIsolationLevel() == IsolationLevel::SERIALIZABLE || 
    //   txn->GetIsolationLevel() == IsolationLevel::REPEATABLE_READ || 
    //   txn->GetIsolationLevel() == IsolationLevel::READ_COMMITTED)) {
    //     txn->SetState(TransactionState::ABORTED);
    //     throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
    // }
    // txn->SetState(TransactionState::GROWING);
    // LockDataId lock_id(tab_fd, rid, LockDataType::RECORD);
    // if (txn->GetLockSet()->count(lock_id) == 0) { // if not exist
    //     txn->GetLockSet()->insert(lock_id); // add into lock set
    //     auto lock_request = new LockRequest(txn->GetTransactionId(), LockMode::SHARED);
    //     lock_table_[lock_id].request_queue_.push_back(*lock_request);
    //     lock_table_[lock_id].IX_lock_num_++;    
    //     // X is not compatible with any locks
    //     while (lock_table_[lock_id].group_lock_mode_ != GroupLockMode::NON_LOCK) {
    //         lock_table_[lock_id].cv_.wait(latch);
    //     }
    //     lock_table_[lock_id].group_lock_mode_ = GroupLockMode::X; // NON_LOCK -> X
    //     lock_request->granted_ = true;
    //     lock_table_[lock_id].cv_.notify_all();
    //     return true;
    // }
    // // if exist (only earse one element, so don't consider iterator bug)
    // for (auto request = lock_table_[lock_id].request_queue_.begin(); request != lock_table_[lock_id].request_queue_.end(); request++) {
    //     if (request->txn_id_ == txn->GetTransactionId()) {
    //         if (request->lock_mode_ == LockMode::EXLUCSIVE) // if the same as before, return directly
    //             return true;
    //         if(lock_table_[lock_id].upgrading_) { // if upgrading, abort and throw
    //             txn->SetState(TransactionState::ABORTED);
    //             throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //         }
    //         // While upgrading, only the following transitions should be allowed: 
    //         // IS -> [S, X, IX, SIX] S -> [X, SIX] IX -> [X, SIX] SIX -> [X]
    //         // except X, all mode can upgrade to X
    //         lock_table_[lock_id].upgrading_ = true;
    //         lock_table_[lock_id].request_queue_.erase(request);
    //         lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::EXLUCSIVE));
    //         lock_table_[lock_id].group_lock_mode_ = GroupLockMode::X;
    //         return true;
    //         //txn->SetState(TransactionState::ABORTED);
    //         //throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //     }
    // }
    // assert(false);
}

/**
 * 申请表级读锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockSharedOnTable(Transaction *txn, int tab_fd) {
    return true;
    // std::unique_lock<std::mutex> latch{latch_};
    // if(txn->GetState() == TransactionState::ABORTED || txn->GetState() == TransactionState::COMMITTED)
    //     return false;
    // if(txn->GetIsolationLevel() == IsolationLevel::READ_UNCOMMITTED){
    //     txn->SetState(TransactionState::ABORTED);
    //     // throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_READ_UNCOMMITTED);
    //     assert(false);
    // }
    // if((txn->GetState() == TransactionState::SHRINKING) && (
    //   txn->GetIsolationLevel() == IsolationLevel::SERIALIZABLE || 
    //   txn->GetIsolationLevel() == IsolationLevel::REPEATABLE_READ)) {
    //     txn->SetState(TransactionState::ABORTED);
    //     throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
    // }
    // txn->SetState(TransactionState::GROWING);
    // LockDataId lock_id(tab_fd, LockDataType::TABLE);
    // if (txn->GetLockSet()->count(lock_id) == 0) { // if not exist
    //     txn->GetLockSet()->insert(lock_id); // add into lock set
    //     auto lock_request = new LockRequest(txn->GetTransactionId(), LockMode::SHARED);
    //     lock_table_[lock_id].request_queue_.push_back(*lock_request);
    //     lock_table_[lock_id].shared_lock_num_++;
    //     // S is not compatible with X/IX/SIX
    //     while (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::X ||
    //              lock_table_[lock_id].group_lock_mode_ == GroupLockMode::IX ||
    //              lock_table_[lock_id].group_lock_mode_ == GroupLockMode::SIX) {
    //         lock_table_[lock_id].cv_.wait(latch);
    //     }
    //     lock_table_[lock_id].group_lock_mode_ = GroupLockMode::S; // S/IS/NON_LOCK -> S
    //     lock_request->granted_ = true;
    //     lock_table_[lock_id].cv_.notify_all();
    //     return true;
    // }
    // // if exist (only earse one element, so don't consider iterator bug)
    // for (auto request = lock_table_[lock_id].request_queue_.begin(); request != lock_table_[lock_id].request_queue_.end(); request++) {
    //     if (request->txn_id_ == txn->GetTransactionId()) {
    //         if (request->lock_mode_ == LockMode::SHARED) // if the same as before, return directly
    //             return true;
    //         if(lock_table_[lock_id].upgrading_) { // if upgrading, abort and throw
    //             txn->SetState(TransactionState::ABORTED);
    //             throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //         }
    //         // While upgrading, only the following transitions should be allowed: 
    //         // IS -> [S, X, IX, SIX] S -> [X, SIX] IX -> [X, SIX] SIX -> [X]
    //         if(request->lock_mode_ == LockMode::INTENTION_SHARED) {
    //             lock_table_[lock_id].upgrading_ = true;
    //             lock_table_[lock_id].request_queue_.erase(request);
    //             lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::SHARED));
    //             lock_table_[lock_id].group_lock_mode_ = GroupLockMode::S;
    //             return true;
    //         } else if (request->lock_mode_ == LockMode::INTENTION_EXCLUSIVE) {
    //             lock_table_[lock_id].upgrading_ = true;
    //             lock_table_[lock_id].request_queue_.erase(request);
    //             lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::S_IX));
    //             lock_table_[lock_id].group_lock_mode_ = GroupLockMode::SIX;
    //             return true;
    //         }
    //         // else abort and throw
    //         txn->SetState(TransactionState::ABORTED);
    //         throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //     }
    // }
    // assert(false);
}

/**
 * 申请表级写锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockExclusiveOnTable(Transaction *txn, int tab_fd) {
    return true;
    // std::unique_lock<std::mutex> latch{latch_};
    // if(txn->GetState() == TransactionState::ABORTED || txn->GetState() == TransactionState::COMMITTED)
    //     return false;
    // if((txn->GetState() == TransactionState::SHRINKING) && (
    //   txn->GetIsolationLevel() == IsolationLevel::SERIALIZABLE || 
    //   txn->GetIsolationLevel() == IsolationLevel::REPEATABLE_READ || 
    //   txn->GetIsolationLevel() == IsolationLevel::READ_COMMITTED)) {
    //     txn->SetState(TransactionState::ABORTED);
    //     throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
    // }
    // txn->SetState(TransactionState::GROWING);
    // LockDataId lock_id(tab_fd, LockDataType::TABLE);
    // if (txn->GetLockSet()->count(lock_id) == 0) { // if not exist
    //     txn->GetLockSet()->insert(lock_id); // add into lock set
    //     auto lock_request = new LockRequest(txn->GetTransactionId(), LockMode::SHARED);
    //     lock_table_[lock_id].request_queue_.push_back(*lock_request);
    //     lock_table_[lock_id].IX_lock_num_++;
    //     // X is not compatible with any lock
    //     while (lock_table_[lock_id].group_lock_mode_ != GroupLockMode::NON_LOCK) {
    //         lock_table_[lock_id].cv_.wait(latch);
    //     }
    //     lock_table_[lock_id].group_lock_mode_ = GroupLockMode::X; // NON_LOCK -> X
    //     lock_request->granted_ = true;
    //     lock_table_[lock_id].cv_.notify_all();
    //     return true;
    // }
    // // if exist (only earse one element, so don't consider iterator bug)
    // for (auto request = lock_table_[lock_id].request_queue_.begin(); request != lock_table_[lock_id].request_queue_.end(); request++) {
    //     if (request->txn_id_ == txn->GetTransactionId()) {
    //         if (request->lock_mode_ == LockMode::EXLUCSIVE) // if the same as before, return directly
    //             return true;
    //         if(lock_table_[lock_id].upgrading_) { // if upgrading, abort and throw
    //             txn->SetState(TransactionState::ABORTED);
    //             throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //         }
    //         // While upgrading, only the following transitions should be allowed: 
    //         // IS -> [S, X, IX, SIX] S -> [X, SIX] IX -> [X, SIX] SIX -> [X]
    //         // except X, all mode can upgrade to X
    //         lock_table_[lock_id].upgrading_ = true;
    //         lock_table_[lock_id].request_queue_.erase(request);
    //         lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::EXLUCSIVE));
    //         lock_table_[lock_id].group_lock_mode_ = GroupLockMode::X;
    //         return true;
    //         //txn->SetState(TransactionState::ABORTED);
    //         //throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //     }
    // }
    // assert(false);
}

/**
 * 申请表级意向读锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockISOnTable(Transaction *txn, int tab_fd) {
    return true;
    // std::unique_lock<std::mutex> latch{latch_};
    // if(txn->GetState() == TransactionState::ABORTED || txn->GetState() == TransactionState::COMMITTED)
    //     return false;
    // if(txn->GetIsolationLevel() == IsolationLevel::READ_UNCOMMITTED){
    //     txn->SetState(TransactionState::ABORTED);
    //     // throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_READ_UNCOMMITTED);
    //     assert(false);
    // }
    // if((txn->GetState() == TransactionState::SHRINKING) && (
    //   txn->GetIsolationLevel() == IsolationLevel::SERIALIZABLE || 
    //   txn->GetIsolationLevel() == IsolationLevel::REPEATABLE_READ)) {
    //     txn->SetState(TransactionState::ABORTED);
    //     throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
    // }
    // txn->SetState(TransactionState::GROWING);
    // LockDataId lock_id(tab_fd, LockDataType::TABLE);
    // if (txn->GetLockSet()->count(lock_id) == 0) { // if not exist
    //     txn->GetLockSet()->insert(lock_id); // add into lock set
    //     auto lock_request = new LockRequest(txn->GetTransactionId(), LockMode::SHARED);
    //     lock_table_[lock_id].request_queue_.push_back(*lock_request);
    //     // IS is not compatible with X
    //     while (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::X) {
    //         lock_table_[lock_id].cv_.wait(latch);
    //     }
    //     // only when NON_LOCK, change to IS
    //     if (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::NON_LOCK)
    //         lock_table_[lock_id].group_lock_mode_ = GroupLockMode::IS; // NON_LOCK -> IS
    //     lock_request->granted_ = true;
    //     lock_table_[lock_id].cv_.notify_all();
    //     return true;
    // }
    // // if exist (only earse one element, so don't consider iterator bug)
    // for (auto request = lock_table_[lock_id].request_queue_.begin(); request != lock_table_[lock_id].request_queue_.end(); request++) {
    //     if (request->txn_id_ == txn->GetTransactionId()) {
    //         if (request->lock_mode_ == LockMode::INTENTION_SHARED) // if the same as before, return directly
    //             return true;
    //         if(lock_table_[lock_id].upgrading_) { // if upgrading, abort and throw
    //             txn->SetState(TransactionState::ABORTED);
    //             throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //         }
    //         // While upgrading, only the following transitions should be allowed: 
    //         // IS -> [S, X, IX, SIX] S -> [X, SIX] IX -> [X, SIX] SIX -> [X]
    //         // abort and throw
    //         txn->SetState(TransactionState::ABORTED);
    //         throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //     }
    // }
    // assert(false);
}

/**
 * 申请表级意向写锁
 * @param txn 要申请锁的事务对象指针
 * @param tab_fd 目标表的fd
 * @return 返回加锁是否成功
 */
bool LockManager::LockIXOnTable(Transaction *txn, int tab_fd) {
    return true;
    // std::unique_lock<std::mutex> latch{latch_};
    // if(txn->GetState() == TransactionState::ABORTED || txn->GetState() == TransactionState::COMMITTED)
    //     return false;
    // if((txn->GetState() == TransactionState::SHRINKING) && (
    //   txn->GetIsolationLevel() == IsolationLevel::SERIALIZABLE || 
    //   txn->GetIsolationLevel() == IsolationLevel::REPEATABLE_READ || 
    //   txn->GetIsolationLevel() == IsolationLevel::READ_COMMITTED)) {
    //     txn->SetState(TransactionState::ABORTED);
    //     throw TransactionAbortException(txn->GetTransactionId(), AbortReason::LOCK_ON_SHIRINKING);
    // }
    // txn->SetState(TransactionState::GROWING);

    // LockDataId lock_id(tab_fd, LockDataType::TABLE);
    // // 查找已经申请锁
    // if (txn->GetLockSet()->count(lock_id) == 0) { // if not exist
    //     txn->GetLockSet()->insert(lock_id); // add into lock set
    //     auto lock_request = new LockRequest(txn->GetTransactionId(), LockMode::INTENTION_EXCLUSIVE);
    //     lock_table_[lock_id].request_queue_.push_back(*lock_request);
    //     // IX is not compatible with S/SIX/X
    //     while (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::S ||
    //              lock_table_[lock_id].group_lock_mode_ == GroupLockMode::SIX ||
    //              lock_table_[lock_id].group_lock_mode_ == GroupLockMode::X) {
    //         lock_table_[lock_id].cv_.wait(latch);
    //     }
    //     lock_request->granted_ = true;
    //     if (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::IS ||
    //         lock_table_[lock_id].group_lock_mode_ == GroupLockMode::NON_LOCK)
    //         lock_table_[lock_id].group_lock_mode_ = GroupLockMode::IX; // IS/NON_LOCK -> IX
    //     lock_table_[lock_id].cv_.notify_all();
    //     return true;
    // }
    // // if exist (only earse one element, so don't consider iterator bug)
    // for (auto request = lock_table_[lock_id].request_queue_.begin(); request != lock_table_[lock_id].request_queue_.end(); request++) {
    //     if (request->txn_id_ == txn->GetTransactionId()) {
    //         if (request->lock_mode_ == LockMode::INTENTION_EXCLUSIVE) // if the same as before, return directly
    //             return true;
    //         if(lock_table_[lock_id].upgrading_) { // if upgrading, abort and throw
    //             txn->SetState(TransactionState::ABORTED);
    //             throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //         }
    //         // While upgrading, only the following transitions should be allowed: 
    //         // IS -> [S, X, IX, SIX] S -> [X, SIX] IX -> [X, SIX] SIX -> [X]
    //         if(lock_table_[lock_id].group_lock_mode_ == GroupLockMode::IS) {
    //             lock_table_[lock_id].upgrading_ = true;
    //             lock_table_[lock_id].request_queue_.erase(request);
    //             lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::INTENTION_EXCLUSIVE));
    //             if (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::IS ||
    //                 lock_table_[lock_id].group_lock_mode_ == GroupLockMode::NON_LOCK)
    //                 lock_table_[lock_id].group_lock_mode_ = GroupLockMode::IX; // IS/NON_LOCK -> IX
    //             return true;
    //         } else if(lock_table_[lock_id].group_lock_mode_ == GroupLockMode::S) {
    //             lock_table_[lock_id].upgrading_ = true;
    //             lock_table_[lock_id].request_queue_.erase(request);
    //             lock_table_[lock_id].request_queue_.push_back(LockRequest(txn->GetTransactionId(), LockMode::S_IX));
    //             if (lock_table_[lock_id].group_lock_mode_ == GroupLockMode::IS ||
    //                 lock_table_[lock_id].group_lock_mode_ == GroupLockMode::IX || 
    //                 lock_table_[lock_id].group_lock_mode_ == GroupLockMode::S ||
    //                 lock_table_[lock_id].group_lock_mode_ == GroupLockMode::NON_LOCK)
    //                 lock_table_[lock_id].group_lock_mode_ = GroupLockMode::SIX; // IS/NON_LOCK -> IX
    //             return true;
    //         }
    //         txn->SetState(TransactionState::ABORTED);
    //         throw TransactionAbortException(txn->GetTransactionId(), AbortReason::UPGRADE_CONFLICT);
    //     }
    // }
    // assert(false);
}

/**
 * 释放锁
 * @param txn 要释放锁的事务对象指针
 * @param lock_data_id 要释放的锁ID
 * @return 返回解锁是否成功
 */
bool LockManager::Unlock(Transaction *txn, LockDataId lock_data_id) {
    return true;
    // std::unique_lock<std::mutex> lock(latch_);
    // txn->SetState(TransactionState::SHRINKING);
    // if (txn->GetLockSet()->count(lock_data_id) != 0) {
    //     auto request = lock_table_[lock_data_id].request_queue_.begin();
    //     // erase request of this txn
    //     while (request != lock_table_[lock_data_id].request_queue_.end()) {
    //         if (request->txn_id_ == txn->GetTransactionId())
    //             request = lock_table_[lock_data_id].request_queue_.erase(request);
    //         else
    //             request++;
    //     }
    //     // update group lock mode
    //     GroupLockMode new_lock_mode = GroupLockMode::NON_LOCK;
    //     for (request = lock_table_[lock_data_id].request_queue_.begin(); request != lock_table_[lock_data_id].request_queue_.end(); request++) {
    //         if (request->granted_) {
    //             // While upgrading, only the following transitions should be allowed: 
    //             // IS -> [S, X, IX, SIX] S -> [X, SIX] IX -> [X, SIX] SIX -> [X]
    //             if (request->lock_mode_ == LockMode::EXLUCSIVE) {
    //                 new_lock_mode = GroupLockMode::X;
    //                 break;
    //             } else if (request->lock_mode_ == LockMode::S_IX) {
    //                 new_lock_mode = GroupLockMode::SIX;
    //             } else if (request->lock_mode_ == LockMode::INTENTION_EXCLUSIVE && new_lock_mode != GroupLockMode::SIX) {
    //                 new_lock_mode = (new_lock_mode == GroupLockMode::S) ? GroupLockMode::SIX : GroupLockMode::IX;
    //             } else if (request->lock_mode_ == LockMode::SHARED && new_lock_mode != GroupLockMode::SIX) {
    //                 new_lock_mode = (new_lock_mode == GroupLockMode::IX) ? GroupLockMode::SIX : GroupLockMode::S;
    //             } else if (request->lock_mode_ == LockMode::INTENTION_SHARED && (new_lock_mode == GroupLockMode::NON_LOCK)) {
    //                 new_lock_mode = GroupLockMode::IS;
    //             }
    //         }
    //     }
    //     lock_table_[lock_data_id].group_lock_mode_ = new_lock_mode;
    //     lock_table_[lock_data_id].cv_.notify_all();
    //     return true;
    // } 
    // return false;
}