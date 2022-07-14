#include <stdio.h>
#include <iostream>

// 平衡二叉树结点定义
typedef struct AVLNode {
    int key;                          //数据域
    int balance;                      //平衡因子
    struct AVLNode *lchild, *rchild;  //左右孩子
} AVLNode, *AVLTree;

// 平衡二叉树插入：
// 找最小不平衡子树，将其调整为平衡二叉树