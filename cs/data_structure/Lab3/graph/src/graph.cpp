#include "../include/graph.h"
#include<iostream>

void graph::init(int u[],int v[],int w[])
{
    for(int i = 0 ; i < edge_num; i++){
        //初始化边节点
        struct Edge* vedge = (struct Edge*)malloc(sizeof(struct Edge)); //关键点：要合理分配内存
        vedge -> adj = v[i];
        vedge -> weight = w[i];
        vedge -> next = nullptr;

        // Vertex unode = vertex[u[i]];错误写法 这是浅拷贝
        //判断是否是第一个节点并连接链表
        if(vertex[u[i]].head){
            vedge -> next = vertex[u[i]].head;
            vertex[u[i]].head = vedge;
        }else{
            vertex[u[i]].head = vedge;
        }

        //无向图，再进行反向操作
        struct Edge* uedge = (struct Edge*)malloc(sizeof(struct Edge)); 
        uedge -> adj = u[i];
        uedge -> weight = w[i];
        uedge -> next = nullptr;

        if(vertex[v[i]].head){
            uedge -> next = vertex[v[i]].head;
            vertex[v[i]].head = uedge;
        }else{
            vertex[v[i]].head = uedge;
        }
    }
}
void graph::dfs(int s)
{
    // Vertex node = vertex[s];
    if(vertex[s].visited) return; //已经被访问过，直接返回

    vertex[s].visited = 1; //标记该节点被访问；
    std:: cout << s  << " ";

    for(struct Edge* edge = vertex[s].head; edge != nullptr; edge = edge -> next){ //深度遍历连接的节点
        dfs(edge -> adj);
    }

}