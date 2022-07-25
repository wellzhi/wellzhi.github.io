typedef int ElemType;
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
// 奇数趟：最大元素移到顺序表表尾
// 偶数趟：最小元素移动顺序表表头
// 反复执行上述，知道整个顺序表递增有序
void BubbleSort(ElemType A[], int n) {
    int low = 0, high = n - 1;
    bool flag = true;
    while (low < high && flag) {
        flag = false;
        for (int i = low; i < high; i++) {
            if (A[i] > A[i + 1]) {
                swap(A[i], A[i + 1]);
                flag = true;
            }
        }
        high--;
        for (int j = high; j > low; j--) {
            if (A[j] < A[j - 1]) {
                swap(A[j], A[j - 1]);
                flag = true;
            }
        }
        low++;
    }
}