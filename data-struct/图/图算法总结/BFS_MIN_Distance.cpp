// 求顶点u到其他顶点的最短路径
void BFS_MIN_Distance(Graph G, int u) {
    // d[i]表示从u到i结点的最短路径
    for (int i = 0; i < G.vexnum; i++) {
        d[i] = ∞;      // 初始化路径长度
        path[i] = -1;  // 最短路径从哪个顶点过来
    }
    d[u] = 0;              // 从u出发
    visited[u] = TRUE;     // 标识u已被访问
    EnQueue(Q, u);         // u入队
    while (!isEmpty(Q)) {  // BFS算法主过程
        DeQueue(Q, u);     // 队头元素u出队
        for (w = FirstNeighbor(G, u); w >= 0; w = NextNeighbor(G, u, w)) {
            if (!visited[w]) {      // w为u的尚未访问的领接顶点
                d[w] = d[u] + 1;    // 路径长度加1
                path[w] = u;        // u->w
                visited[w] = TRUE;  // 标记已访问
                EnQueue(Q, w);      // 顶点w入队
            }
        }
    }
}