#pragma once

#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "execution_defs.h"
#include "record/rm.h"
#include "system/sm.h"
#include "common/context.h"

struct TabCol {
    std::string tab_name;
    std::string col_name;

    friend bool operator<(const TabCol &x, const TabCol &y) {
        return std::make_pair(x.tab_name, x.col_name) < std::make_pair(y.tab_name, y.col_name);
    }
};

struct Value {
    ColType type;  // type of value
    union {
        int int_val;      // int value
        float float_val;  // float value
    };
    std::string str_val;  // string value

    std::shared_ptr<RmRecord> raw;  // raw record buffer

    void set_int(int int_val_) {
        type = TYPE_INT;
        int_val = int_val_;
    }

    void set_float(float float_val_) {
        type = TYPE_FLOAT;
        float_val = float_val_;
    }

    void set_str(std::string str_val_) {
        type = TYPE_STRING;
        str_val = std::move(str_val_);
    }

    void init_raw(int len) {
        assert(raw == nullptr);
        raw = std::make_shared<RmRecord>(len);
        if (type == TYPE_INT) {
            assert(len == sizeof(int));
            *(int *)(raw->data) = int_val;
        } else if (type == TYPE_FLOAT) {
            assert(len == sizeof(float));
            *(float *)(raw->data) = float_val;
        } else if (type == TYPE_STRING) {
            if (len < (int)str_val.size()) {
                throw StringOverflowError();
            }
            memset(raw->data, 0, len);
            memcpy(raw->data, str_val.c_str(), str_val.size());
        }
    }
};

enum CompOp { OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE };

struct Condition {
    TabCol lhs_col;   // left-hand side column
    CompOp op;        // comparison operator
    bool is_rhs_val;  // true if right-hand side is a value (not a column)
    TabCol rhs_col;   // right-hand side column
    Value rhs_val;    // right-hand side value
};

struct SetClause {
    TabCol lhs;
    Value rhs;
};

/** task2: Order by **/
struct OrderBy {
    std::string order_col;     // Order by column name
    std::string order_type;    // Desc or Asc
};

struct Record_Cmp {
    std::vector<int> order_col;             // Order by column index
    std::vector<int> order_type;            // 0: Desc, 1: Asc
    Record_Cmp(std::vector<int> order_col_, std::vector<int> order_type_) : order_col(order_col_), order_type(order_type_) {}
    bool operator()(const std::vector<std::string>& record1, const std::vector<std::string>& record2 ) {
        int len = order_col.size();
        for (int i = 0; i < len; i++) {
            if (order_type[i] == 0) { // Desc
                if (record1[order_col[i]] > record2[order_col[i]]) {
                    return true;
                } else if (record1[order_col[i]] < record2[order_col[i]]) {
                    return false;
                }
            } else { // Asc
                if (record1[order_col[i]] < record2[order_col[i]]) {
                    return true;
                } else if (record1[order_col[i]] > record2[order_col[i]]) {
                    return false;
                }
            }
        }
        return false;
    }
};
/********************/
class QlManager {
   private:
    SmManager *sm_manager_;
    // add some variables here, used for sort
    
   public:
    QlManager(SmManager *sm_manager) : sm_manager_(sm_manager) {}

    void insert_into(const std::string &tab_name, std::vector<Value> values, Context *context);

    void delete_from(const std::string &tab_name, std::vector<Condition> conds, Context *context);

    void update_set(const std::string &tab_name, std::vector<SetClause> set_clauses, 
                    std::vector<Condition> conds, Context *context);

    void select_from(std::vector<TabCol> sel_cols, const std::vector<std::string> &tab_names,
                     std::vector<Condition> conds, std::vector<OrderBy> orders, int limit_num, Context *context);

   private:
    TabCol check_column(const std::vector<ColMeta> &all_cols, TabCol target);
    std::vector<ColMeta> get_all_cols(const std::vector<std::string> &tab_names);
    std::vector<Condition> check_where_clause(const std::vector<std::string> &tab_names,
                                              const std::vector<Condition> &conds);
    int get_indexNo(std::string tab_name, std::vector<Condition> curr_conds);
};
