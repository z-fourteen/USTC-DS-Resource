#include "walk.h"
#include <cmath>
#include <vector>

namespace DATA_STRUCTURE {

void walk::print_para() {
    std::cout << steps << std::endl;
    for (int i = 0; i < gold_num; i++) {
        std::cout << *(gold + 2 * i) << " " << *(gold + 2 * i + 1) << " " << *(gold_value + i) << std::endl;
    }
}

int walk::compute_distance(int i, int x, int y) {
    int gold_x = gold[2 * i], gold_y = gold[2 * i + 1];
    int d_x = abs(gold_x - x), d_y = abs(gold_y - y);
    return  (d_x + 1) / 2 + (d_y + 1 ) / 2; 
}

int walk::get_value() {

    unordered_set<std::string> visited;

    int max_value = 0;

    //用全局集合存储已有状态
    visited.insert("0,0,0,0,0"); //初始状态
    element now_state = {0,0,0,0,0}; //初始状态，从原点出发
    q.push(now_state);

    while(!q.empty()){
        now_state = q.top();
        q.pop();

        // 更新最大值
        max_value = std::max(max_value, now_state.value);
        //如果步数已经超过，则直接跳过该种情况
        if(now_state.steps >= steps) continue;
        
        //尝试前往每一个金币位置
        for(int i = 0; i < gold_num; i++){
            int dist = compute_distance(i, now_state.x, now_state.y);
            // 使用位掩码记录哪些金币已被收集
            // 如果当前路径已经收集过第 i 个金币，则跳过
            if (now_state.mask & (1 << i)) {
                continue;
            }
            if(now_state.steps + dist <= steps && dist > 0){//dist>0防止原地打转
                element next_state;
                next_state.steps = now_state.steps + dist;
                next_state.value = now_state.value + gold_value[i];
                next_state.x = gold[2 * i];
                next_state.y = gold[2 * i + 1];
                next_state.mask = now_state.mask | (1 << i);
                std::string state_key = std::to_string(next_state.steps) + "," +
                                        std::to_string(next_state.value) + "," +
                                        std::to_string(next_state.x) + "," +
                                        std::to_string(next_state.y) + "," +
                                        std::to_string(next_state.mask);
                //如果该状态没有出现过，则加入队列和集合
                if(visited.find(state_key)) continue;
                visited.insert(state_key);
                q.push(next_state);
            }
        }

    }
    return max_value;
}

}