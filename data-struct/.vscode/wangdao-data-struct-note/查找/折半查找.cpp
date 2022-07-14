// 折半查找，也称为 二分查找，仅适用于有序的顺序表
typedef int ElemType;
typedef struct {     // 查找表的数据结构（顺序表）
    ElemType* elem;  // 动态数据的地址
    int TableLen;    // 表长
} SSTable;

// 折半查找（假设待查找表为升序）
int BinarySearch(SSTable L, ElemType key) {
    int low = 0, high = L.TableLen - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;  // 取中间位置
        if (L.elem[mid] == key) {
            return mid;  // 查找成功则返回所在的位置
        } else if (L.elem[mid] > key) {
            high = mid - 1;  // 从前半部分继续查找
        } else {
            low = mid + 1;  // 从后半部分继续查找
        }
    }
    return -1;  // 查找失败，返回-1；
}