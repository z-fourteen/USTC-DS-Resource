//图的定义
#define MVNUM 100 //最大顶点数
typedef char VertexType; //顶点的数据类型
typedef int EdgeType; //边的权值类型
typedef struct ArcNode{
    int adjvex; //该弧所指向的顶点的位置
    struct ArcNode *nextarc; //指向下一条弧的指针
    EdgeType info; //该弧相关信息的指针
} ArcNode;
typedef struct VNode{
    VertexType data; //顶点信息
    ArcNode *firstarc; //指向第一条依附该顶点的弧的指针
} VNode, AdjList[MVNUM];
typedef struct{
    AdjList vertices; //邻接表
    int vexnum, arcnum; //图的当前顶点数和弧数
} ALGraph;

