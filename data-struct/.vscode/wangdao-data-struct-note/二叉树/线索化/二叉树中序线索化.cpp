#include <iostream>
typedef int ElemType;
typedef struct ThreadNode {
    ElemType data;
    struct ThreadNode *lchild, *rchild;
    int ltag, rtag;  // 左右线索标志：0普通结点、1线索结点
} ThreadNode, *ThreadTree;

ThreadNode* pre = NULL;  // 全局变量pre,指向当前访问结点的前驱

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

// 中序遍历二叉树，一遍遍历一遍线索化
void InThread(ThreadTree T) {
    if (T != NULL) {
        InThread(T->lchild);  // 中序遍历左子树
        visit(T);             // 访问根节点
        InThread(T->rchild);  // 中序遍历右子树
    }
}

// 中序线索化二叉树（给定一个二叉树，将其线索化）
void CreateInThread(ThreadTree T) {
    pre = NULL;       // pre初始化为NULL
    if (T != NULL) {  // 非空二叉树才能线索化
        InThread(T);  // 中序线索化二叉树
        if (pre->rchild == NULL) {
            pre->rtag = 1;  // 处理遍历的最后一个结点
        }
    }
}
