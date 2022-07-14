#include <iostream>
typedef int ElemType;
typedef struct ThreadNode {
    ElemType data;
    struct ThreadNode *lchild, *rchild;
    int ltag, rtag;
} ThreadNode, *ThreadTree;
ThreadNode* pre = NULL;

void visit(ThreadNode* q) {
    if (q->lchild == NULL) {  // 左子树为空，建立前驱线索
        q->lchild = pre;
        q->ltag = 1;
    }
    if (pre != NULL && pre->rchild == NULL) {
        pre->rchild = q;  // 建立前驱结点的后继线索
        pre->rtag = 1;
    }
    pre = q;
}

// 后序遍历二叉树，一遍遍历一遍线索化
void PostThread(ThreadTree T) {
    if (T != NULL) {
        PostThread(T->lchild);  // 遍历左子树
        PostThread(T->rchild);  // 遍历右子树
        visit(T);               // 访问根节点
    }
}

void CreatePostThread(ThreadTree T) {
    pre = NULL;         // pre初始为NULL
    if (T != NULL) {    // 非空二叉树才能线索化
        PostThread(T);  // 后序线索化二叉树
        if (pre->rchild == NULL) {
            pre->rtag = 1;  // 处理遍历的最后一个结点
        }
    }
}