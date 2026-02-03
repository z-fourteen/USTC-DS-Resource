#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <utility>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 };


class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        /*
            1.实现思路：队列+广度优先搜索 基于地址的算法
                地址计算：left -> idx*2, right -> idx*2+1. 
                每层宽度计算：width = last_index - first_index + 1.
            2.难点：层数的确定
        */
        if (root == nullptr) return 0;
        unsigned long long maxWidth = 0;
        std::queue<std::pair<TreeNode*, unsigned long long>> q; //采用ull存储数字 防止地址过大溢出
        q.push({root, 1ULL});
        while (!q.empty()) {
            size_t levelSize = q.size(); // 当前层的节点数
            unsigned long long firstIndex = q.front().second;
            unsigned long long lastIndex = firstIndex;
            for (size_t i = 0; i < levelSize; ++i) {
                auto p = q.front(); q.pop();
                TreeNode* node = p.first;
                unsigned long long idx = p.second;
                lastIndex = idx;
                if (node->left)  q.push({node->left,  idx * 2ULL});
                if (node->right) q.push({node->right, idx * 2ULL + 1ULL});
            }
            unsigned long long width = lastIndex - firstIndex + 1ULL;
            if (width > maxWidth) maxWidth = width;
        }
        return static_cast<int>(maxWidth);
    }
};

// Helper test runner
static bool run_test(TreeNode* root, int expected, const std::string &name) {
    Solution sol;
    int got = sol.widthOfBinaryTree(root);
    std::cout << name << ": expected=" << expected << ", got=" << got;
    if (got == expected) {
        std::cout << " [PASS]" << std::endl;
        return true;
    } else {
        std::cout << " [FAIL]" << std::endl;
        return false;
    }
}

int main() {
    int pass = 0, total = 0;

    // Test 1: empty tree
    ++total;
    if (run_test(nullptr, 0, "empty tree")) ++pass;

    // Test 2: single node
    {
        TreeNode* t = new TreeNode(1);
        ++total; if (run_test(t, 1, "single node")) ++pass;
        delete t;
    }

    // Test 3: full tree of height 3 -> width should be 4 (level 3 has 4 nodes)
    {
        TreeNode* n4 = new TreeNode(4);
        TreeNode* n5 = new TreeNode(5);
        TreeNode* n6 = new TreeNode(6);
        TreeNode* n7 = new TreeNode(7);
        TreeNode* n2 = new TreeNode(2, n4, n5);
        TreeNode* n3 = new TreeNode(3, n6, n7);
        TreeNode* root = new TreeNode(1, n2, n3);
        ++total; if (run_test(root, 4, "full tree height 3")) ++pass;
        // cleanup
        delete n4; delete n5; delete n6; delete n7; delete n2; delete n3; delete root;
    }

    // Test 4: example [1,3,2,5,3,null,9] -> expected width 4
    {
        TreeNode* a5 = new TreeNode(5);
        TreeNode* a3 = new TreeNode(3);
        TreeNode* a9 = new TreeNode(9);
        TreeNode* left = new TreeNode(3, a5, a3);
        TreeNode* right = new TreeNode(2, nullptr, a9);
        TreeNode* root = new TreeNode(1, left, right);
        ++total; if (run_test(root, 4, "sparse example [1,3,2,5,3,null,9]")) ++pass;
        delete a5; delete a3; delete a9; delete left; delete right; delete root;
    }

    // Test 5: skewed tree (all left), width always 1
    {
        TreeNode* a = new TreeNode(1);
        TreeNode* cur = a;
        for (int i = 2; i <= 6; ++i) {
            cur->left = new TreeNode(i);
            cur = cur->left;
        }
        ++total; if (run_test(a, 1, "skewed-left tree")) ++pass;
        // cleanup chain
        cur = a;
        while (cur) { TreeNode* nxt = cur->left; delete cur; cur = nxt; }
    }

    std::cout << "\nTests passed: " << pass << " / " << total << std::endl;
    return (pass == total) ? 0 : 1;
}