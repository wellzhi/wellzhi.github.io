// 建立大根堆
void BuildMaxHeap(int A[], int len) {
    for (int i = len / 2; i > 0; i--) {
        HeadAdjust(A, i, len);
    }
}

// 将以k为根的子树调整为大根堆
void HeadAdjust(int A[], int k, int len) {
    A[0] = A[k];                             // A[0]暂存子树的根结点
    for (int i = 2 * k; i <= len; i *= 2) {  // 沿key较大的子结点向下筛选
        if (i < len && A[i] < A[i + 1]) {
            i++;  // 取key子结点较大者的下标
        }
        if (A[0] >= A[i]) {
            break;  // 根key比子结点下标都大，无须调整
        } else {
            A[k] = A[i];  // 将A[i]调整到双亲结点上
            k = i;        // 修改k值，以便继续向下筛选
        }
    }
    A[k] = A[0];  // 被筛选结点的值放入最终位置
}

/**
 * 堆排序完整逻辑
 **/
void HeapSort(int A[], int len) {
    BuildMaxHeap(A, len);            // 初始建堆
    for (int i = len; i > 1; i--) {  // n-1趟交换和建堆过程
        swap(A[i], A[1]);            // 将堆顶元素和堆底元素交换
        HeadAdjust(A, 1, i - 1);     // 把剩余的待排序元素整理成堆
    }
}