#include "calculator.h"
#include "queue.h"
#include <cmath>    //添加数学库
#include <cctype>  // 添加标准字符处理头文件
#include <stdexcept>

namespace DATA_STRUCTURE {

std::string calculator::get_expr() {
    return expr;
}

bool calculator::isop(char c){
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
bool calculator::legal() {
    /*
     * 括号检查实现思路：遇到左括号入栈 遇到右括号出栈 检查栈是否为空
     */
    stack<char> br; 
    int len = expr.size();
    for(int i = 0; i < len ; i++){
        //检查连续操作符
        if(i < len - 1 && isop(expr[i]) && isop(expr[i+1])){
            return false;
        }
        if (expr[i] == '('){
            br.push(expr[i]);
        }else if(expr[i] == ')'){
            if(br.empty()){
                return false; //右括号多于左括号 不匹配
            }
            br.pop();
        }
    }
    return br.empty();
}

int calculator::priority_regular(char c) {
    switch(c) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

int calculator::priority(char c1, char c2) {
    if(priority_regular(c1) == 3 && priority_regular(c2) == 3){
        return false;
    }
    return priority_regular(c1) >= priority_regular(c2);
}

struct calculator::element calculator::read_num() {
    std:: string num;
    bool has_decimal = false;

    while(expr_index < expr.size() && 
            (std::isdigit(expr[expr_index]) || expr[expr_index] == '.')){
        if(expr[expr_index] == '.'){
            if(has_decimal) break; //多个小数点 停止读取
            has_decimal = true;
        }
        num += expr[expr_index];
        expr_index++;
    }
    if(has_decimal){
        double ans = std:: stod(num);
        return {1, 0, ans};
    }else{
        int ans = std::stoi(num);
        return {0, ans, 0};
    }
}

struct calculator::element calculator::operate(struct calculator::element e1, char op, struct calculator::element e2) {
    double num1 = (e1.flag == 0) ? static_cast<double>(e1.num_int) : e1.num_double;
    double num2 = (e2.flag == 0) ? static_cast<double>(e2.num_int) : e2.num_double;
    double result = 0.0;
    
    switch(op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
            if(num2 == 0) throw std::runtime_error("Division by zero");
            result = num1 / num2; 
            break;
        case '^': result = std::pow(num1, num2); break;
        default: throw std::runtime_error("Unknown operator");
    }
    
    // 如果结果是整数且没有精度损失，返回整数
    if(std::abs(result - std::round(result)) < 1e-10) {
        return {0, static_cast<int>(std::round(result)), 0.0};
    } else {
        return {1, 0, result};
    }
}

struct calculator::element calculator::get_ans() {
    expr_index = 0;  // 重置索引
    
    while(expr_index < expr.size()) {
        // 跳过空格
        if(std::isspace(expr[expr_index])) {
            expr_index++;
            continue;
        }
        
        // 如果是数字
        if(std::isdigit(expr[expr_index])) {
            num.push(read_num());
        }
        // 如果是左括号
        else if(expr[expr_index] == '(') {
            op.push('(');
            expr_index++;
        }
        // 如果是右括号
        else if(expr[expr_index] == ')') {
            while(!op.empty() && op.top() != '(') {
                // 执行运算
                auto e2 = num.top(); num.pop();
                auto e1 = num.top(); num.pop();
                char c = op.top(); op.pop();
                num.push(operate(e1, c, e2));
            }
            if(!op.empty()) op.pop();  // 弹出左括号
            expr_index++;
        }
        // 如果是运算符
        else if(isop(expr[expr_index])) {
            while(!op.empty() && op.top() != '(' && 
                  priority(op.top(), expr[expr_index])) {
                auto e2 = num.top(); num.pop();
                auto e1 = num.top(); num.pop();
                char c = op.top(); op.pop();
                num.push(operate(e1, c, e2));
            }
            op.push(expr[expr_index]);
            expr_index++;
        }
    }
    
    // 处理剩余运算符
    while(!op.empty()) {
        auto e2 = num.top(); num.pop();
        auto e1 = num.top(); num.pop();
        char c = op.top(); op.pop();
        num.push(operate(e1, c, e2));
    }
    
    if(num.empty()) {
        return {0, 0, 0.0};
    }
    return num.top();
}
}