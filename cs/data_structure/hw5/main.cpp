#include <iostream>
#include <queue>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

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
    int findSecondMinimumValue(TreeNode* root) {
        /*实现思路： 广度优先遍历 每一层遍历结束之后获取第二小的数*/
        
        std::queue<TreeNode*> q;//队列：存储节点以实现广度优先遍历
        q.push(root);
        int min1 = root -> val, min2 = 0;
        while(!q.empty()){
            int width = q.size();//获取该层节点总数
            for(int i = 0; i < width; i++){
                TreeNode* node = q.front();
                q.pop();
                if(min2 = 0 && node -> val > min1){
                    min2 = node -> val;
                }
                if(min2 != 0 && node -> val > min1 && node -> val < min2){
                    min2 = node -> val;
                }
                if(node -> left) q.push(node -> left);
                if(node -> right) q.push(node -> right);
            }
        }
        return min2 ? min2 : -1;
    }
};

int main() {
    // Test cases
    TreeNode* root1 = new TreeNode(2, new TreeNode(2), new TreeNode(5, new TreeNode(5), new TreeNode(7)));
    Solution sol;
    int result1 = sol.findSecondMinimumValue(root1);
    std::cout << "Test 1: Expected 5, got " << result1 << std::endl;

    // Clean up
    delete root1->left;
    delete root1->right->left;
    delete root1->right->right;
    delete root1;

    return 0;
}