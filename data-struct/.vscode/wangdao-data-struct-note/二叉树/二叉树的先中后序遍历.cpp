/*
一. 二叉树的先、中、后序遍历：
1.先序（根）遍历
2.中序（根）遍历
3.后序（根）遍历
二. 二叉树的层次遍历：
*/

#include <stdio.h>
typedef int ElemType;
typedef struct BiTNode {              // 二叉树的结构定义
    ElemType data;                    // 数据域
    struct BiTNode *lchild, *rchild;  // 左右孩子指正
} BiTNode, *BiTree;                   // 结点：BiTNode，二叉树： *BiTree

/*
先序遍历：
（1）若二叉树为空，则什么也不做
（2）若二叉树非空：
 先访问根节点
 再先序遍历左子树
 再先序遍历右子树
 */
void visit(BiTree T) {}
void PreOrder(BiTree T) {
    if (T != NULL) {
        visit(T);             // 访问根节点
        PreOrder(T->lchild);  // 递归遍历左子树
        PreOrder(T->rchild);  // 递归遍历右子树
    }
}

// 中序遍历
void InOrder(BiTree T) {
    if (T != NULL) {
        InOrder(T->lchild);
        visit(T);
        InOrder(T->rchild);
    }
}

void PostOrder(BiTree T) {
    if (T != NULL) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        visit(T);
    }
}

/*
考点：1. 给定一个二叉树，确定遍历序列
2.结合栈应用：算数表达式的分析树（简言之，就是一个算式组成的二叉树）
- 前缀表达式（先序遍历）
- 中缀表达式（中序遍历）
- 后缀表达式（后序遍历）
3. 求二叉树的深度（应用）
 */
// 求二叉树的深度
int treeDepth(BiTree T) {
    if (T == NULL) {
        return 0;
    }
    int l = treeDepth(T->lchild);
    int r = treeDepth(T->rchild);

    // 树的深度 = Max(左子树深度，右子树深度) + 1
    return l > r ? l + 1 : r + 1;
}
