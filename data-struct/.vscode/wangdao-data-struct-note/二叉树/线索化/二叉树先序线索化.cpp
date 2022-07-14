#include <iostream>
typedef int ElemType;
typedef struct ThreadNode {
    ElemType data;
    struct ThreadNode *lchild, *rchild;
    int ltag, rtag;
} ThreadNode, *ThreadTree;

ThreadNode* pre = NULL;

// 遍历并线索化
void visit(ThreadNode* q) {
    if (q->lchild == NULL) {
        q->lchild = pre;  // 左子树为空，建立前驱线索
        q->ltag = 1;
    }
    if (pre != NULL && pre->rchild == NULL) {
        pre->rchild = q;  // 建立前驱结点的后继线索
        pre->rtag = 1;
    }
    pre = q;
}

// 先序线索化
void PreThread(ThreadTree T) {
    if (T != NULL) {
        visit(T);            // 先访问根节点
        if (T->ltag == 0) {  // lchild不是前驱线索
            PreThread(T->lchild);
        }
        PreThread(T->rchild);
    }
}

void CreatPreThread(ThreadTree T) {
    pre = NULL;        // 全局变量初始化为NULL
    if (T != NULL) {   // 非空二叉树才能线索化
        PreThread(T);  // 先序线索化二叉树
        if (pre->rchild == NULL) {
            pre->rtag = 1;  // 处理遍历的最后一个结点
        }
    }
}