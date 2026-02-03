#include "../include/HuffmanTree.h"

// 构造函数
HuffmanTreeNode::HuffmanTreeNode(char ch, int freq) : character(ch), frequency(freq) {
    left = right = nullptr;
}

// 重写 getValue 方法，返回字符
string HuffmanTreeNode::getValue() {
    return string(1, character);  // 返回字符
}

// 获取节点频率
int HuffmanTreeNode::getFrequency() const {
    return frequency;
}

// 构建哈夫曼树
HuffmanTreeNode* HuffmanTreeNode::buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    // 创建最小堆
    priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, freq_cmp> minHeap;

    // 将所有字符和频率加入堆中
    for(const auto& pair : freqMap){
        HuffmanTreeNode* node = new HuffmanTreeNode();
        node -> character = pair.first;
        node -> frequency = pair.second;
        minHeap.push(node);
    }
    
    
    // 合并最小的两个节点，直到堆中只剩下一个节点
    while(minHeap.size() > 1){
        HuffmanTreeNode* val1 = minHeap.top();
        minHeap.pop();
        HuffmanTreeNode* val2 = minHeap.top();
        minHeap.pop();
        HuffmanTreeNode* newVal = new HuffmanTreeNode();
        newVal -> frequency = val1 -> getFrequency() + val2 -> getFrequency();
        newVal -> left = val1;
        newVal -> right = val2;
        minHeap.push(newVal);
    }

    // 返回哈夫曼树的根节点
    return minHeap.top();
}

// 打印哈夫曼树的编码（先序遍历）
void HuffmanTreeNode::printHuffmanCodes(HuffmanTreeNode* root, string code) {
    char left = '0', right = '1';
    if(!root) return;
    if(!root -> left && ! root -> right){ //遇到叶子节点则打印
        cout << root -> character << " " << code << "\n";
    }
    printHuffmanCodes(static_cast<HuffmanTreeNode*>(root -> left),code + left);
    printHuffmanCodes(static_cast<HuffmanTreeNode*>(root -> right), code + right);
}
