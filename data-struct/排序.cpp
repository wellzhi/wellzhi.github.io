
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

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
/**
 *  快速排序★★★★
 * 1. 选基准元素
 * 2. 小基左提，大基右提
 * 3. low、high相等，一分为二
 * 4. 递归至完成
 */

/**
 * 一分为二
 * 用第一个元素（基准元素）将待排序序列划分为左右两个部分
 */
int Partition(int A[], int low, int high) {
    int pivot = A[low];   // 第一个元素作为枢轴
    while (low < high) {  // 用low、high搜索枢轴的最终位置
        while (low < high && A[high] >= pivot) {
            --high;
        }
        A[low] = A[high];  // 比枢轴小的元素移动到左端
        while (low < high && A[low] <= pivot) {
            ++low;
        }
        A[high] = A[low];  // 比枢轴大的元素移动到右端
    }

    A[low] = pivot;  // 枢轴元素存放到最终位置
    return low;      // 返回存放枢轴的最终位置
}

// 快速排序算法
void QuickSort(int A[], int low, int high) {
    if (low < high) {
        int pivotPos = Partition(A, low, high);  // 一次划分
        QuickSort(A, low, pivotPos - 1);         // 划分左子表
        QuickSort(A, pivotPos + 1, high);        // 划分右子表
    }
}
// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

/**
 * 简单选择排序
 * 稳定性：不稳定
 * 适用性：顺序表、链表
 * 空间复杂度：O（1）
 * 时间复杂度：无论是否有序，时间复杂度均为O（n^2)
 *
 **/
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
void SelectSort(int A[], int n) {
    for (int i = 0; i < n - 1; i++) {  // 一共进行n-1趟比较
        int min = i;                   //记录最小元素的位置
        for (int j = i + 1; j < n; j++) {  // 在A[i,...,n-1]中选择最小的元素
            if (A[i] < A[min]) {
                min = j;  // 更新最小元素的位置
            }
        }

        if (min != i) {
            swap(A[i], A[min]);
        }
    }
}