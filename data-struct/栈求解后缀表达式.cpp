#include <iostream>
#include <stack>
#include <vector>
using namespace std;
/**
 * 后缀表达式求值（逆波兰式求值）
 * 1. 准备一个栈，从左向右扫描后缀表达式。
 * 2.
 *遇到操作数就入栈,遇到运算符则出栈两个操作数，先出栈的作为右操作数,后栈的作为左操作数,然后运算之后把结果继续压入栈内
 * 3. 重复以上步骤，最后栈中的栈顶元素就是最终计算结果
 *
 **/
class Solution {
   public:
    // 操作数计算之后，将计算结果压入栈
    void calculate(int& opnd1, string& op, int& opnd2, int& result);
    int evalRPN(vector<string>& tokens);
};

void Solution::calculate(int& opnd1, string& op, int& opnd2, int& result) {
    if (op == "+") {
        result = opnd1 + opnd2;
    }
    if (op == "-") {
        result = opnd1 - opnd2;
    }
    if (op == "*") {
        result = opnd1 * opnd2;
    }
    if (op == "/") {
        result = opnd1 / opnd2;
    }
}

int Solution::evalRPN(vector<string>& tokens) {
    stack<int> ret;  // 结果栈
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].size() == 1 && (tokens[i] == "+") || tokens[i] == "-" ||
            tokens[i] == "*" || tokens[i] == "/") {
            int opnd1, opnd2, result;  // 两个操作数和结果
            string op;                 // 运算符
            op = tokens[i];
            opnd2 = ret.top();
            ret.pop();  // 先出来的是右操作数
            opnd1 = ret.top();
            ret.pop();  // 先出来的是右操作数
            calculate(opnd1, op, opnd2, result);
            ret.push(result);  // 计算结果仍然压入栈
        } else {
            // atoi()函数将数字格式的字符串转换为整数类型。例如，将字符串“12345”转换成数字12345。
            int temp = atoi(tokens[i].c_str());
            ret.push(temp);
        }
    }
    return ret.top();  // 栈顶元素是最终结果
}
