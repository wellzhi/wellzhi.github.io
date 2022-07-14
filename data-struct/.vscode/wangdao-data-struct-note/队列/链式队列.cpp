#include <stdio.h>
#include <iostream>
typedef int ElemType;
// 链式队列结点
typedef struct LinkNode {
    ElemType data;          // 数据域
    struct LinkNode* next;  // 后继结点指针
} LinkNode;
// 链式队列
typedef struct {
    LinkNode *front, *rear;  // 队头、队尾指针
} LinkQueue;

// 初始化(带头结点)
void InitQueue(LinkQueue& Q) {
    // 初始化时，front、rear都指向头结点
    Q.front = Q.rear = (LinkNode*)malloc(sizeof(LinkNode));
    Q.front->next = NULL;  // 头结点后继结点为NULL
}
bool IsEmpty(LinkQueue Q) {
    if (Q.front = Q.rear) {
        return true;
    } else {
        return false;
    }
}

// 新元素入队（带头结点）
void EnQueue(LinkQueue& Q, ElemType x) {
    LinkNode* s = (LinkNode*)malloc(sizeof(LinkNode));
    s->data = x;
    s->next = NULL;
    Q.rear->next = s;  // 新结点插入到rear之后
    Q.rear = s;        // 修改队尾指针
}

// 队头元素出队（带头结点）
bool DeQueue(LinkQueue& Q, ElemType& x) {
    if (Q.front == Q.rear) {  // 空队列
        return false;
    }
    LinkNode* p = Q.front->next;  // 队头结点
    x = p->data;                  // 用变量x返回队头元素
    Q.front->next = p->next;      // 修改头结点的next指针
    if (Q.rear == p) {            // 此次是最后一个结点出队
        Q.rear = Q.front;         // 修改rear指针
    }
    free(p);  // 释放出队结点空间
    return true;
}

void main() {
    LinkQueue Q;
    InitQueue(Q);
}

//  =========不带头结点====================================
void InitQueueNonHead(LinkQueue& Q) {
    // 初始化时，front、rear都指向NULL
    Q.front = NULL;
    Q.rear = NULL;
}


void EnQueueNonHead(LinkQueue& Q, ElemType x) {
    LinkNode* s = (LinkNode*)malloc(sizeof(LinkNode));
    s->data = x;
    s->next = NULL;
    if (Q.front == NULL) {  // 在空队列中插入第一个元素
        Q.front = s;        // 修改队头、队尾指针
        Q.rear = s;
    } else {
        Q.rear->next = s;  // 新结点插入到队尾之后
        Q.rear = s;        // 修改rear指针
    }
}

bool DeQueueNonHead(LinkQueue& Q, ElemType& x) {
    if (Q.front == NULL) {  // 空队
        return false;
    }
    LinkNode* p = Q.front->next;  // p之指向此次出队的队头结点
    x = p->data;                  // 用变量返回队头元素
    Q.front = p->next;            // 队头指针修改（后移）
    if (Q.rear == p) {            // 如果此次是最后一个结点出队
        Q.front = NULL;           // front 指向null
        Q.rear = NULL;            // rear 指向null
    }
    free(p);  // 释放结点空间
    return true;
}

bool IsEmptyNonHead(LinkQueue Q) {
    if (Q.front == NULL) {
        return true;
    } else {
        return false;
    }
}