typedef int ElemType;
// 孩子兄弟表示法(链式存储，二叉链表)
typedef struct CSNode {
    ElemType data;                            // 数据域
    struct CSNode *firstchild, *nextsibling;  // 第一个孩子和右兄弟指针

} CSNode, *CSTree;
