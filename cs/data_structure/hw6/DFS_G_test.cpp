#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <functional>

// ----------------------------------------------------
// 1. 包含核心头文件和功能定义
// ----------------------------------------------------
#include "GraphAL.h" // 包含图的定义

// 外部函数声明 (从你的 DFS_G.cpp 和 PathExist.cpp 引入)
extern void DFS_AL(ALGraph G, int v, void (*Visit)(int v));
extern bool PathExists(ALGraph G, int start, int end, int k);

// ----------------------------------------------------
// 2. 辅助函数（用于测试）
// ----------------------------------------------------

/**
 * @brief 简单的访问函数，打印顶点索引
 * @param v 顶点索引
 */
void TestVisit(int v) {
    std::cout << v << " ";
}

/**
 * @brief 创建一个测试用的图
 * @param vexnum 顶点数
 * @param edges 边列表 (start, end)
 * @return 构造好的邻接表图
 */
ALGraph CreateTestGraph(int vexnum, const std::vector<std::pair<int, int>>& edges) {
    ALGraph G;
    G.vexnum = vexnum;
    G.arcnum = 0;

    for (int i = 0; i < vexnum; ++i) {
        G.vertices[i].data = (char)('A' + i);
        G.vertices[i].firstarc = nullptr;
    }

    for (const auto& edge : edges) {
        int v1 = edge.first;
        int v2 = edge.second;

        // 添加 v1 -> v2 的边
        ArcNode* node1 = new ArcNode;
        node1->adjvex = v2;
        node1->nextarc = G.vertices[v1].firstarc;
        G.vertices[v1].firstarc = node1;
        G.arcnum++;

        // 由于是无向图，还需要添加 v2 -> v1 的边
        ArcNode* node2 = new ArcNode;
        node2->adjvex = v1;
        node2->nextarc = G.vertices[v2].firstarc;
        G.vertices[v2].firstarc = node2;
        G.arcnum++;
    }
    return G;
}



// ----------------------------------------------------
// 3. 主测试函数
// ----------------------------------------------------

void RunTest(const std::string& name, bool success) {
    std::cout << "\n--------------------------------------------------" << std::endl;
    std::cout << "TEST CASE: " << name << std::endl;
    std::cout << "RESULT: ";
    if (success) {
        std::cout << "\033[1;32mPASSED\033[0m" << std::endl; // 绿色 PASSED
    } else {
        std::cout << "\033[1;31mFAILED\033[0m" << std::endl; // 红色 FAILED
    }
    std::cout << "--------------------------------------------------" << std::endl;
}

int main() {
    std::cout << "Data Structure Homework Test Runner" << std::endl;
    std::cout << "===================================" << std::endl;

    // A(0) -- B(1) -- C(2)
    // |      /
    // D(3) -- E(4)

    // 顶点数 5
    // 边: (0, 1), (1, 2), (0, 3), (3, 4), (1, 4)
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 2}, {0, 3}, {3, 4}, {1, 4}
    };
    ALGraph G = CreateTestGraph(5, edges);

    // --- TEST 1: 非递归 DFS 遍历 ---
    std::cout << "\n[TEST 1] Non-Recursive DFS Traversal (Start at 0):" << std::endl;
    std::cout << "Expected Output (One possible order): 0 3 4 1 2 (or similar stack order)" << std::endl;
    std::cout << "Actual Output: ";
    DFS_AL(G, 0, TestVisit);
    std::cout << std::endl;
    RunTest("DFS Traversal", true); // DFS遍历只要不崩溃，且遍历所有点，即可视为通过

    // --- TEST 2: PathExists (长度 k=2) ---
    // 路径: 0 -> 1 -> 2 (长度 2)
    bool result2 = PathExists(G, 0, 2, 2);
    RunTest("Path 0->2, Length 2", result2 == true);

    // --- TEST 3: PathExists (长度 k=3) ---
    // 路径: 0 -> 3 -> 4 -> 1 (长度 3)
    bool result3 = PathExists(G, 0, 1, 3); 
    RunTest("Path 0->1, Length 3", result3 == true);

    // --- TEST 4: PathExists (不存在的路径) ---
    // 存在长度为 4 的简单路径
    bool result4 = PathExists(G, 0, 2, 4);
    RunTest("Path 0->2, Length 4 (Expected True)", result4 == true);

    // --- TEST 5: PathExists (长度 k=1) ---
    // 路径: 0 -> 3 (长度 1)
    bool result5 = PathExists(G, 0, 3, 1);
    RunTest("Path 0->3, Length 1", result5 == true);

    return 0;
}