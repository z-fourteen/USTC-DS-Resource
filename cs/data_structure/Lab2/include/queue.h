#pragma once

#include "list.h"

namespace DATA_STRUCTURE {

template<class data_base>
class queue : public list<data_base> {
    public:
        queue() = default;
        ~queue() = default;

        bool empty() override {
            return top_index == tail_index;
        }
        void pop() override {
            top_index ++;
        }
        data_base top() override {
            data_base tmp = list_data[top_index];
            return tmp;
        }
        void push(data_base push_data) override {
            list_data[tail_index % MAX_LINK_LIST_NUM] = push_data;
            tail_index ++;
        }
        void clear() override {
            top_index = 0;
            tail_index = 0;
        }
    private:
        int top_index = 0;
        int tail_index = 0;
        data_base list_data[MAX_LINK_LIST_NUM];
};

}
