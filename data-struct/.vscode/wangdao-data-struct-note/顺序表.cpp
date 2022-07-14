// 顺序表的静态分配和动态分配
// 顺序表（静态分配）
#define MaxSize 10
typedef int ElemType;
typedef struct {
    ElemType data
        [MaxSize];  // 用静态的"数组"存放数据元素，存储空间是静态的，数组满了就无法更改
    int length;
} SqList;

// 顺序表（动态分配）
/**
 * 动态申请和释放内存空间————malloc、free函数
 * 申请：L.data = (ElemType*)malloc(sizeof(ElemType)*InitSize);
 *       malloc返回一个指针，需要强制转换类型为你定义的数据元素类型指针
 * 释放：free(p); p：指针
 * # include <stdlib.h>
 **/
#include <stdlib.h>
#define InitSize 10  // 顺序表的初始长度
typedef struct {
    ElemType* data;  // 指示动态分配数组的指针
    int MaxSize;     // 顺序表的最大容量
    int length;      // 顺序表的当前长度
} SeqList;           // 顺序表的类型定义（动态分配方式）

void InitList(SeqList& L) {
    L.data = (int*)malloc(InitSize * sizeof(int));
    L.length = 0;
    L.MaxSize = InitSize;
}

void IncreaseSize(SeqList& L, int len) {
    int* p = L.data;
    L.data = (int*)malloc((L.MaxSize + len) * sizeof(int));
    for (int i = 0; i < L.length; i++) {
        L.data[i] = p[i];  // 将数据复制到新内存区域
    }
    L.MaxSize = L.MaxSize + len;  // 顺序表的最大长度增加len
    free(p);                      // 释放原来的内存空间
}