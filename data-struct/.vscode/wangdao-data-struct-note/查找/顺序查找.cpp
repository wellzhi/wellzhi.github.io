typedef int ElemType;
typedef struct {
    ElemType* elem;  // 动态数组地址
    int TableLen;    // 表长
} SSTable;

int SearchSeq(SSTable ST, ElemType key) {
    ST.elem[0] = key;  // 哨兵，放在数组下标为0的地方
    int i;
    for (i = ST.TableLen; ST.elem[i] != key; --i) {
        return i;  // 查找成功，则返回元素下标；查找失败，则返回0；
    }
}