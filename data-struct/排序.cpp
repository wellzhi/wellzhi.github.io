
// 直接插入排序(中间变量temp)
void InsertSort(int A[], int n) {  // 数组A、数组长度n
    int i, j, temp;
    for (i = 1; i < n; i++) {  // 将各元素插入前面已经排好序的序列中
        if (A[i] < A[i - 1]) {  // 若A[i]关键字小于前驱
            temp = A[i];        // 用temp暂存A[i]
            // 检查前面已排好序的元素
            for (j = i - 1; j >= 0 && A[j] > temp; --j) {
                A[j + 1] = A[j];  // 所有大于temp的元素都向后挪位
            }
            A[j + 1] = temp;  // 复制到插入位置
        }
    }
}

// 直接插入排序（带哨兵）
// 数组下标为0的元素为哨兵
void InsertSort_Sentinel(int A[], int n) {
    int i, j;
    for (i = 2; i <= n; i++) {
        // 第一个位置为哨兵，从第三个位置开始，将各元素插入到前面已经排好序的序列中
        if (A[i] < A[i - 1]) {
            A[0] = A[i];
            for (j = i - 1; A[j] > A[0]; --j) {
                A[j + 1] = A[j];
            }
            A[j + 1] = A[0];
        }
    }
}

// 直接插入排序（折半插入排序）
void InsertSort_HalfInsert(int A[], int n) {
    int i, j, low, mid, high;
    for (i = 2; i <= n; i++) {
        A[0] = A[i];
        low = 1;  // 第一个位置为哨兵，从第二个位置开始为待排序元素
        high = i - 1;        // i前面序列的最后一个元素
        while (low <= high)  // 折半查找（默认递增有序）
        {
            mid = (low + high) / 2;  // 取中间点
            if (A[mid] > A[0]) {
                high = mid - 1;  // 查找左子表
            } else {
                low = mid + 1;  // 查找右子表
            }
        }

        // 将位置i与high之间（不包含边界）的元素，依次往后挪动一个位置
        for (j = i - 1; j >= high + 1; --j) {
            A[j + 1] = A[j];  // 依次后移元素，空出插入位置
        }
        A[high + 1] = A[0];  // 插入元素
    }
}