#include <iostream>

void InsertSort(int A[], int n) {  // A中共n个数据元素
    int i, j, temp;
    for (i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) {  // A[i]关键字小于前驱
            temp = A[i];        // temp暂存待插入元素
            for (j = i - 1; j >= 0 && A[j] > temp; j--) {
                printf("%d---%d\n", A[j - 1], A[j]);
                A[j] = A[j - 1];  //所有大于temp的元素都向后挪
                printf("%d---%d\n", A[j - 1], A[j]);
            }
            A[j + 1] = temp;  //复制到插入位置
        }
    }
}

// 0相当于哨兵
void InsertSort2(int a[], int n) {
    for (int i = 2; i <= n; i++) {
        a[0] = a[i];
        int j;
        for (j = i - 1; a[0] < a[j]; j--) {
            a[j + 1] = a[j];
        }
        a[j + 1] = a[0];
    }
}

int Partition(int a[], int low, int high) {
    int pivot = a[low];
    while (low < high) {
        while (low < high && a[high] >= pivot) {
            high--;
        }
        a[low] = a[high];

        while (low < high && a[low] <= pivot) {
            low++;
        }
        a[high] = a[low];
    }
    a[low] = pivot;
    return low;
}

void QuickSort(int a[], int low, int high) {
    if (low < high) {
        int pivotPos = Partition(a, low, high);
        QuickSort(a, low, pivotPos - 1);
        QuickSort(a, pivotPos + 1, high);
    }
}

int main() {
    int A[] = {9, 7, 5, 3, 1};
    QuickSort(A, 0, 4);
    for (int i = 0; i < 5; i++) {
        printf("%d\n", A[i]);
    }
    free(A);
    return 0;
}
