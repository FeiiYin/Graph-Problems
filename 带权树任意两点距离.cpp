/*
 * @Samaritan_infi
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N = 3e5 + 5;

struct Edge {
    int u, val;
    Edge () {}
    Edge (int a, int b) {
        u = a; val = b;
    }
};

vector<Edge> edge[N];

/// LCA
int pre[32][N]; /// Ancestor nodes
int dep[N];     /// depth of nodes
int dis[N];     /// distance to root

void dfs (int now, int pa) {
    pre[0][now] = pa;
    for (Edge e : edge[now]) if (e.u != pa) {
        dep[e.u] = dep[now] + 1;
        dis[e.u] = dis[now] + e.val;
        dfs(e.u, now);
    }
}

void init_lca (int n) {
    dfs(1, -1); /// st, -1 根据起点标号dfs
    for (int k = 0; k < 20; k ++) {
        for (int v = 0; v < n; v ++) {
            if (pre[k][v])
                pre[k + 1][v] = pre[k][ pre[k][v] ];
        }
    }
}

int lca (int u, int v) {
    if (dep[u] > dep[v]) swap(u, v);
    for (int k = 0; k < 20; k ++)
        if ((dep[v] - dep[u]) & (1 << k))
            v = pre[k][v];
    if (u == v) return u;
    for (int k = 19; k >= 0; k --) if (pre[k][u] != pre[k][v]) {
        u = pre[k][u];
        v = pre[k][v];
    }
    return pre[0][u];
}

int main () {
    int n; scanf("%d", &n);
    int l, r, v;
    for (int i = 1; i < n; i ++) {
        scanf("%d %d %d", &l, &r, &v);
        edge[l].push_back(Edge(r, v));
        edge[r].push_back(Edge(l, v));
    }
    init_lca(n);
    int op; scanf("%d", &op);
    while (op --) {
        scanf("%d %d", &l, &r);
        int k = lca(l, r);
        printf("lca is %d\n", k);
        printf("distance between two nodes %d\n", dis[l] + dis[r] - 2 * dis[k]);
    }
    return 0;
}
/*
5
1 2 3
1 3 2
3 4 1
3 5 6
5
2 4
2 5
3 5
4 4
1 3
*/
