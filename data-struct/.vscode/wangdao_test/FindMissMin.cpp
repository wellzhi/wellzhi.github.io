#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

int findMissMin(int A[], int n) {
    int i, *B;
    B = (int*)malloc(sizeof(int) * n);  // 分配空间
    memset(B, 0, sizeof(int) * n);      // 初始化为0
    for (int i = 0; i < n; i++) {
        if (A[i] > 0 && A[i] < n) {
            // 0不存在、1存在
            B[A[i] - 1] = 1;  // 若A[i]的值介于1~n之间，则标记数组B
        }
    }

    for (int i = 0; i < n; i++) {
        if (B[i] == 0) {
            break;  // 扫描数组B，找到目标值（第一个值为0的数组下标）
        }
    }

    return i + 1;  // 返回结果
}