#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

class Ackermann{
    public:
        //递归实现Ackermann函数
        int Ack(int m, int n){
            if(m == 0){
                return n + 1;
            }else if(n == 0){
                return Ack(m-1, 1);
            }else{
                return Ack(m - 1, Ack(m, n - 1));
            }
        }
        //非递归实现Ackermann函数
        int Ack_non_recursive(int m, int n){
            /*
                实现思路：
                1. 通过将m压入栈中 控制计算次数
                2.将n作为结果随计算过程更新
            */
            stack<int> s;
            s.push(m);
            
            while (!s.empty()) {
                m = s.top();
                s.pop();
                
                if (m == 0) {
                    n = n + 1;
                } else if (n == 0) {
                    s.push(m - 1);
                    n = 1;
                } else {
                    s.push(m - 1);
                    s.push(m);
                    n = n - 1;
                }
            }
            return n;
        }
};

class List_Recursion{
    public:
    //链表节点定义
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };
 
    //辅助函数，递归查找链表中最大的数
    int find_max_num(ListNode* p, int max_num){
        if(p  == nullptr){
            return max_num;
        }
        if(p -> val > max_num){
            max_num = p -> val;
        }
        return find_max_num(p -> next, max_num);
    }
    int list_max_num(ListNode* p){
        int max_num = p -> val;
        return find_max_num(p -> next, max_num);//调用辅助函数
    }   

    //递归计算节点个数
    int node_num(ListNode* p){
        if(p == nullptr){
            return 0;
        }
        return 1 + node_num(p -> next);
    }
    
    //递归计算链表的和
    int list_sum(ListNode* p){
        if(p == nullptr){
            return 0;
        }
        return p -> val + list_sum(p -> next);
    }
    //计算链表整数平均值
    int list_avg(ListNode* p){
        return list_sum(p) / node_num(p);
    }
};

class Solution{
    public:
        //辅助函数:将栈中的数字转换为整数
        long to_num(stack<int> temp){
            int exp = 0, num = 0;
            while(!temp.empty()){
                num += temp.top() * pow(10, exp);
                temp.pop();
                exp++;
            }
            return num;
        }
        //辅助函数:根据表达式字符串，计算表达式结果
        long get_res(string s){
            long res = 0;
            stack<int> temp;
            char op = '+';  //存放待使用运算符，默认为'+'
            long num = 0; //存放待使用数字
            for(char c : s){
                if(c == '+' || c == '-'){
                    //遇到运算符，将栈中的数字转换为整数并计算，更新op
                    num = to_num(temp);
                    res += op == '+' ? num : -num;
                    num = 0;
                    op = c;
                }else{
                    //遇到数字，将数字压入栈中
                    temp.push(c - '0');
                }
            }
            //计算最后一个数字
            num = to_num(temp);
            res += op == '+' ? num : -num;
            return res;
        }
        vector<string> solution(string s){
            //获取数字个数
            int n = s.size(); 
            //初始化，存放表达式结果
            vector<string> res_arr;
            //存放目标数字
            int target = 110;

            //函数：递归计算所有可能的表达式
            void dfs(string s, int i, string exp){
                //递归终止条件: 处理完所有数字之后检验结果是否为目标值
                if(i == n){
                    long ans = get_res(exp);
                    if(ans == target){
                        res.push_back(exp);
                    }
                    return;
                }
                //特殊情况处理：第一位数字直接存入表达式
                if(i == 0){
                    dfs(s, i + 1, s.substr(i, 1));
                }else{
                    dfs(s, i + 1, exp + "+" + s.substr(i, 1));
                    dfs(s, i + 1, exp + "-" + s.substr(i, 1));
                }
            }
            
            //调用递归函数
            dfs(s, 0, "");
            return res_arr;
        } 
};