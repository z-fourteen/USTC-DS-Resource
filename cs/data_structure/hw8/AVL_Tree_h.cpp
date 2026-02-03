#include <iostream>

struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int BF; // Balance Factor
    int height;

    AVLNode(int k) : key(k), left(nullptr), right(nullptr), BF(0), height(1) {}
};

class AVLTree {
public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insertRec(root, key);
    }

    void inorder() {
        inorderRec(root);
        std::cout << std::endl;
    }
    //函数：计算树的高度
    int treeHeight() {
        if (!root) return 0;
        return height(root);
    }
private:
    AVLNode* root;
    AVLNode* insertRec(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);

        if (key < node->key) {
            node->left = insertRec(node->left, key);
        } else if (key > node->key) {
            node->right = insertRec(node->right, key);
        } else {
            return node; // Duplicate keys are not allowed
        }

        updateBalanceFactor(node);
        return balance(node);
    }
    //算法：递归计算节点高度
    int height(AVLNode* node) {
        if (!node) return 0;
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        return std::max(leftHeight, rightHeight) + 1;
    }
    void updateBalanceFactor(AVLNode* node) {
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        node->BF = rightHeight - leftHeight;
    }
    AVLNode* balance(AVLNode* node) {
        if (node->BF < -1) {
            if (node->left && node->left->BF > 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if (node->BF > 1) {
            if (node->right && node->right->BF < 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }
    AVLNode* rotateLeft(AVLNode* z) {
        AVLNode* y = z->right;
        AVLNode* T2 = y->left;

        y->left = z;
        z->right = T2;

        updateBalanceFactor(z);
        updateBalanceFactor(y);

        return y;
    }
    AVLNode* rotateRight(AVLNode* z) {
        AVLNode* y = z->left;
        AVLNode* T3 = y->right;

        y->right = z;
        z->left = T3;

        updateBalanceFactor(z);
        updateBalanceFactor(y);

        return y;
    }
    void inorderRec(AVLNode* node) {
        if (!node) return;
        inorderRec(node->left);
        std::cout << node->key << " ";
        inorderRec(node->right);
    }
};

int main() {
    AVLTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    std::cout << "Inorder traversal of the constructed AVL tree is:\n";
    tree.inorder();
    std::cout << "Height of the AVL tree is: " << tree.treeHeight() << std::endl;
    return 0;
}