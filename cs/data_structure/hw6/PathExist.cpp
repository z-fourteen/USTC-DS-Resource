#include "GraphAL.h"
#include <stack>
#include <functional>
using namespace std;

/*  
    判别无向图中任意给定两顶点之间是否存在长度为k的简单路径
    实现思路：
    1. 使用深度优先搜索（DFS）遍历图，从起始顶点开始。
    2. 在DFS过程中，维护当前路径的长度，并使用一个数组记录已访问的顶点以避免重复访问。
    3. 当路径长度达到k时，检查当前顶点是否为目标顶点。
    4. 如果找到满足条件的路径，则返回true；否则继续搜索，直到所有可能的路径都被探索完毕。

    难点：
        由于是路径的确定而非遍历，所以需要维护visited数组，并在回溯时取消标记，以确保路径的简单性。

*/

bool PathExists(ALGraph G, int start, int end, int k){
    bool visited[MVNUM] = {false}; //访问标记数组
    std:: function<bool(int,int)> DFS = [&](int v, int length) -> bool { //知识点学习：使用lambda表达式定义DFS函数
        if(length == k){
            return v == end; //路径长度为k时，检查是否到达终点
        }
        visited[v] = true; //标记当前顶点为已访问

        for(ArcNode *p = G.vertices[v].firstarc; p != nullptr; p = p->nextarc){
            int adjvex = p->adjvex;
            if(!visited[adjvex]){ //如果邻接顶点未被访问
                if(DFS(adjvex, length + 1)){
                    return true; //找到满足条件的路径
                }
            }
        }

        visited[v] = false; //回溯，取消标记
        return false; //未找到满足条件的路径
    };
    return DFS(start, 0);
}

