#include "../include/dijkstra.h"
#include <iostream>
void dijkstra::ssp(int s)
{
    //TODO:实现以s为起始点的dijkstra算法求最短路径
    //初始化
    for(int i = 0 ; i < g.vertex_num; i++){
        vertex[i].sure = 0;
        vertex[i].path = -1;
        vertex[i].dist = M;
    }
    vertex[s].sure = 1;
    vertex[s].dist = 0;
    for(graph::Edge* edge = g.vertex[s].head; edge != nullptr; edge = edge -> next){
        //更新其他节点的最短路径值
        int newDist = edge -> weight + vertex[s].dist;
        if(newDist < vertex[edge -> adj].dist){ 
            vertex[edge -> adj].path = s;
            vertex[edge -> adj].dist = edge -> weight;
        }  
    }

    
    int minDistIndex = s;
    for(int sureNum = 1; sureNum < g.vertex_num;){
        //寻找未被加入的路径最短的节点
        int minDist = M;
        for(int i = 0 ; i < g.vertex_num; i++){
            if(!vertex[i].sure && vertex[i].dist < minDist){
                minDistIndex = i;
                minDist = vertex[i].dist;
            }
        }
        vertex[minDistIndex].sure = 1;
        sureNum++;

        //更新其他节点的最短路径值
        for(graph::Edge* edge = g.vertex[minDistIndex].head; edge != nullptr; edge = edge -> next){
            if(vertex[edge -> adj].sure) continue;
            int newDist = edge -> weight + vertex[minDistIndex].dist;
            if(newDist < vertex[edge -> adj].dist){ 
                vertex[edge -> adj].path = minDistIndex;
                vertex[edge -> adj].dist = newDist;
            }  
        }
    }
    
    //打印路径
    for(int i = 0 ; i < g.vertex_num; i++){
        print(i);
        std:: cout << "\n";
    }
}
void dijkstra::print(int u)
{
    //TODO:用于输出最短路径的辅助函数，可以使用递归实现

    //递归终止条件，到达源节点
    if(u == -1) return;
    print(vertex[u].path);
    std:: cout << u << " ";
}