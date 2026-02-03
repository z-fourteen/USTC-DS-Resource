#include "GraphAL.h"

/*
    深度优先搜索遍历图的非递归算法
    实现思路：
    1. 使用栈来模拟递归调用栈，存储待访问的顶点。
    2. 从起始顶点开始，将其标记为已访问并入栈。
    3. 当栈不为空时，弹出栈顶顶点，访问该顶点。
    4. 遍历该顶点的所有邻接顶点，对于每个未访问的邻接顶点，标记为已访问并入栈。
    5. 重复步骤3和4，直到栈为空，表示所有可访问的顶点都已被访问。
*/

#include <iostream>
#include <stack>
using namespace std;

void DFS_AL(ALGraph G, int v, void (*Visit)(int v)){
    bool visited[MVNUM] = {false}; //访问标记数组
    stack<int> S; //定义栈
    S.push(v); //将起始顶点入栈
    visited[v] = true; //标记起始顶点为已访问

    while(!S.empty()){
        int current = S.top(); //获取栈顶元素
        S.pop(); //弹出栈顶元素
        Visit(current); //访问当前顶点

        //遍历当前顶点的所有邻接顶点
        for(ArcNode *p = G.vertices[current].firstarc; p != nullptr; p = p->nextarc){
            int adjvex = p->adjvex;
            if(!visited[adjvex]){ //如果邻接顶点未被访问
                S.push(adjvex); //将邻接顶点入栈
                visited[adjvex] = true; //标记邻接顶点为已访问
            }
        }
    }
}