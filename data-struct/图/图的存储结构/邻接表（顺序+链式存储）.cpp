#define MaxVertexNum 100

typedef char VertexType;
typedef int EdgeType;
// 边/弧结构体
typedef struct ArcNode {
    int adjVex;            // 指向哪个结点
    struct ArcNode* next;  // 指向下一条弧的指针
} ArcNode;

// 顶点
typedef struct VNode {
    VertexType data;  //顶点信息
    ArcNode* first;   // 第一条边/弧
} VNode, AdjList[MaxVertexNum];

// 用邻接表存储图
typedef struct {
    AdjList vertices;
    int vexnum, arcnum;
} ALGraph;
