#define MaxVertexNum 100  // 顶点数最大值
typedef int VertexType;

typedef struct ArcNode  // 边表结点 Arc：弧 Vertex：顶点
{
    int adjvex;  // 该弧指向的顶点的位置（顶点表下标）
    struct ArcNode* nextArc;  // 指向下一条弧的指针
} ArcNode;

typedef struct VNode {  // 顶点表结点
    VertexType data;    // 顶点信息
    ArcNode* firstArc;  // 指向第一条依附于该结点的弧的指针
} VNode, AdjList[MaxVertexNum];

typedef struct {
    AdjList vertices;    // 链接表
    int vexnum, arcnum;  // 顶点数、弧数
} Graph;                 // Graph：以邻接表存储的图类型

/**
 *  拓扑排序:
 * 1. 从AOV网中选择一个没有前驱（入度=0）的顶点并输出
 * 2. 从网中上次该顶点和所有以它为起点的有向边
 * 3. 重复1、2直到当前的AOV网为空或当前网中不存在无前驱的顶点为止
 */
bool TopologicalSort(Graph G) {
    InitStack(S);  // 初始化栈，用于存储入度为0的顶点
    for (int i = 0; i < G.vexnum; i++) {
        if (indegree[i] == 0) {  // indegree[i] 顶点-入度数
            Push(S, i);          // 将所有入度为0的顶点入栈
        }
    }
    int count = 0;           //计数，记录当前应输出的顶点数
    while (!IsEmpty(S)) {    // 栈不为空，则存在入度为0的顶点
        Pop(S, i);           // 栈顶元素出栈
        print[count++] = i;  // 输出栈顶元素i
        for (p = G.vertices[i].firstArc; p; p->nextArc) {
            // 将所有i指向的顶点的入度减去1，并且将入度减为0的顶点压入栈S
            int v = p->adjVex;
            if (!(--indegree[v])) {
                Push(S, v);  // 入度为0的结点，入栈
            }
        }
    }
    if (count < G.vexnum) {
        return false;  // 排序失败，有向图中有回路
    } else {
        return true;  // 拓扑排序成功
    }
}