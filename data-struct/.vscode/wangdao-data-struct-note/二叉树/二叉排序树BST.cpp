#include <stdio.h>
#include <iostream>
typedef struct BSTNode {
    int key;
    struct BSTNode *lchild, *rchild;
} BSTNode, *BSTree;

// (非递归实现）在二叉树中查找值为key的结点
BSTNode* BST_Search(BSTree T, int key) {
    while (T != NULL && key != T->key) {
        if (key < T->key) {
            T = T->lchild;
        } else {
            T = T->rchild;
        }
    }
    return T;
}
// (递归实现)在二叉排序树中查找值为Key的结点
BSTNode* BSTSearch(BSTree T, int key) {
    if (T == NULL) {
        return NULL;
    }
    if (key = T->key) {
        return T;
    } else if (key < T->key) {
        return BSTSearch(T->lchild, key);
    } else {
        return BSTSearch(T->rchild, key);
    }
}

// (递归实现)在二叉排序树插入关键字为k的新结点
int BSTInsert(BSTree& T, int k) {
    if (T == NULL) {
        /* code */
        T = (BSTree)malloc(sizeof(BSTNode));
        T->key = k;
        T->lchild = NULL;
        T->rchild = NULL;
        return 1;
    } else if (k == T->key) {
        return 0;  // 树中存在相同关键字的结点，插入失败
    } else if (k < T->key) {
        return BSTInsert(T->lchild, k);
    } else {
        return BSTInsert(T->rchild, k);
    }
}

// 构造二叉排序树
void CreateBST(BSTree& T, int str[], int n) {
    T = NULL;
    int i = 0;
    while (i < n) {
        BSTInsert(T, str[i]);
        i++;
    }
}
