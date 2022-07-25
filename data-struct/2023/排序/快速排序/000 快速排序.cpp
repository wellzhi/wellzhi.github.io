int Partition(int A[], int low, int high) {
    int pivot = A[low];
    while (low < high) {
        while (low < high && A[high] >= pivot) {
            high--;
        }
        A[low] = A[high];  // 比枢轴元素小的元素移动到左端
        while (low < high && A[low] <= pivot) {
            low++;
        }
        A[high] = A[low];  // 比枢轴元素大的元素移动到右端
    }
    A[low] = pivot;  // 枢轴元素放到最终位置
    return low;  // 枢轴元素的最终位置，左边元素小于枢轴元素，右边元素大于枢轴元素
}

void QuickSort(int A[], int low, int high) {
    if (low < high) {
        int pivotPos = Partition(A, low, high);  // 进行一次划分
        QuickSort(A, low, pivotPos - 1);         // 递归划分左边部分
        QuickSort(A, pivotPos + 1, high);        // 递归划分右边部分
    }
}