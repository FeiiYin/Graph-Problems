/// 题意：有最少用多少条边不重复的路径可以覆盖一张无向图。
/// 分析：对于一个连通块（单个点除外），如果奇度数点个数为 k，那么至少需要max{k/2,1}条路径。将奇度数的点两两相连边（虚边），
/// 然后先从奇度数的点出发，搜索由其出发的欧拉回路。需要将遍历的边和其反向边打标记，并在DFS退栈的时候记录边的编号（前向星的存储是访问后加入的边），
/// 若该边是自己添加的虚边，那么说明实际上这次DFS搜索到的是一条欧拉通路，那么结果还需额外+1，所以对所有奇数点DFS过后，得到的结果就是max{k/2,1}。
/// 再从未被访问过的偶数顶点出发搜索由其出发的欧拉回路，每一次DFS就是找到了一条回路。
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e5 + 5;

struct Edge {
    int to, id, next;
    bool f;
} edges[N << 4];

int edge_num, head[N], cnt;
bool vis[N];
vector<int> res[N];
int deg[N];

void init () {
    edge_num = 0;
    cnt = 0;
    memset(deg, 0, sizeof(deg));
    memset(vis, 0, sizeof(vis));
    memset(head, -1, sizeof(head));
}

void add_edge (int u, int v, int id) {
    edges[edge_num].f = 0; edges[edge_num].to = v; edges[edge_num].id = id;
    edges[edge_num].next = head[u]; head[u] = edge_num++;
}

void dfs (int u) {
    vis[u] = true;
    for (int i = head[u]; ~i; i = edges[i].next) {
        int v = edges[i].to, id = edges[i].id;
        if (! edges[i].f) {
            edges[i].f = edges[i^1].f = true;    ///将边和反向边标记
            dfs(v);
            if (id) res[cnt].push_back(-id);     ///退栈记录边的id
            else cnt++;                          ///扫到虚边，那么路径加1
        }
    }
}
void output() {
    printf("%d\n", cnt);
    for (int i = 1; i <= cnt; ++i) {
        printf("%d", res[i].size());
        for (int j : res[i]) printf(" %d", j); puts("");
        res[i].clear();
    }
}

int main() {
    int n, m;
    while(~ scanf("%d %d", &n, &m)) {
        init();
        int u, v;
        for (int i = 1; i <= m; ++i) {
            scanf("%d %d", &u, &v);
            deg[u]++; deg[v]++;
            add_edge(u, v, i);
            add_edge(v, u, -i);
        }
        u = 0;
        for (int i = 1; i <= n; ++i) {
            if(deg[i] & 1) { /// 将奇度数点两两连边
                if (u) {
                    add_edge(u, i, 0);
                    add_edge(i, u, 0);
                    u = 0;
                } else u = i;
            }
        }
        for (int i = 1; i <= n; ++i)
            if(! vis[i] && (deg[i] & 1)) {
                cnt++;
                dfs(i);
                cnt--;
            }
        for (int i = 1; i <= n; ++i)
            if(! vis[i] && deg[i]) {
                cnt++;
                dfs(i);
            }
        output();
    }
    return 0;
}
