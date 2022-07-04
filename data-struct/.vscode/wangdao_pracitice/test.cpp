#include <stdio.h>
#include <iostream>
#include <vector>
#define ElemType char
using namespace std;

typedef struct LNode {
    ElemType data;
    struct LNode* next;
} LNode, *LinkList;

int getLen(LinkList L) {
    int len = 0;
    while ((L->next != NULL)) {
        L = L->next;
        len++;
    }
    return len;
}

LNode* findCommon(LinkList A, LinkList B) {
    int lenA = getLen(A);
    int lenB = getLen(B);

    if (lenA > lenB) {
        for (int i = 0; i < lenA - lenB; i++) {
            A = A->next;
        }
    } else {
        for (int i = 0; i < lenB - lenA; i++) {
            B = B->next;
        }
    }

    while (A != NULL) {
        if (A == B) {
            cout << A->data;
            return A;
        }
        A = A->next;
        B = B->next;
    }
    return NULL;
}

LinkList createHeadList(vector<ElemType> data) {
    if (data.size() == 0) {
        return NULL;
    }
    LNode* head = (LinkList)malloc(sizeof(LNode));
    head->next = NULL;

    LNode* p = head;
    for (int i = 0; i < data.size(); i++) {
        LNode* q = (LNode*)malloc(sizeof(LNode));
        q->data = data[i];
        q->next = NULL;
        p->next = q;
        p = q;
    }
    return head;
}

void printList(LinkList L) {
    while (L != NULL) {
        cout << L->data;
        L = L->next;
    }
    puts("");
}

int main() {
    vector<char> dataA = {'l', 'o', 'a', 'd'};
    vector<char> dataB = {'b', 'e'};
    vector<char> dataC = {'i', 'n', 'g'};
    LinkList headA = createHeadList(dataA);
    LinkList headB = createHeadList(dataB);
    LinkList headC = createHeadList(dataC);

    LNode* p1 = headA->next;
    LNode* p2 = headB->next;
    LNode* p3 = headC->next;

    while (p1->next != NULL) {
        p1 = p1->next;
    }
    p1->next = p3;
    while (p2->next != NULL) {
        p2 = p2->next;
    }
    p2->next = p3;

    cout << "链表A：";
    printList(headA->next);
    cout << "链表B: ";
    printList(headB->next);
    return 0;
}