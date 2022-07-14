
#include <stdio.h>
typedef int ElemType;
typedef struct BiTNode {              // 二叉树的结构定义
    ElemType data;                    // 数据域
    struct BiTNode *lchild, *rchild;  // 左右孩子指正
} BiTNode, *BiTree;                   // 结点：BiTNode，二叉树： *BiTree

// 链式队列结点
typedef struct LinkNode {
    BiTNode* data;
    struct LinkNode* next;
} LinkNode;
// 链式队列定义
typedef struct {
    LinkNode *front, *rear;  // 队头、队尾指针
} LinkQueue;

/*
算法思想：
1.初始化一个辅助队列
2.根节点入队
3.若队列非空，则队头结点出队，访问该结点，并将其左右孩子插入队尾（如果有的话）
4.重复步骤3直至队列为空
 */
void LevelOrder(BiTree T) {
    LinkQueue Q;   // 辅助队列
    InitQueue(Q);  // 初始辅助队列
    BiTree p;
    EnQueue(Q, T);       // 将根节点入队
    while (!IsEmpty(Q))  // 队列不空则循环
    {
        DeQueue(Q, p);  // 队头结点出队
        visit(p);       // 访问出队结点
        if (p->lchild != NULL) {
            EnQueue(Q, p->lchild);  // 有左孩子，入队
        }
        if (p->rchild != NULL) {
            EnQueue(Q, p->rchild);  // 有有孩子，入队
        }
    }
}