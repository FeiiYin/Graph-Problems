/*
 * @Samaritan_infi
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

const int N = 3e5 + 5;
const int INF = 0x3f3f3f3f;
struct Edge {
    int u, val;
    Edge () {}
    Edge (int a, int b) {
        u = a; val = b;
    }
};

vector<Edge> edge[N];
inline void add_edge (int u, int v, int w) {
    edge[u].push_back(Edge(v, w));
    edge[v].push_back(Edge(u, w));
}

/// LCA
int dep[N];     /// depth of nodes
int dfn[N], dfn_num;     /// dfs 序
int top[N], son[N], siz[N], fa[N];
int min_edgelen[N];

bool cmp_by_dfn (int x, int y) { return dfn[x] < dfn[y]; }

void dfs1 (int now, int pa) {
    siz[now] = 1;
    fa[now] = pa;
    for (Edge e : edge[now]) if (e.u != pa) {
        dep[e.u] = dep[now] + 1;
        min_edgelen[e.u] = min(min_edgelen[now], e.val);
        dfs1(e.u, now);
        siz[now] += siz[e.u];
        if (siz[e.u] > siz[ son[now] ]) {
            son[now] = e.u;
        }
    }
}

void dfs2 (int x, int topfa) {
    top[x] = topfa;
    dfn[x] = ++ dfn_num;
    if (! son[x]) return;
    dfs2(son[x], topfa);
    for (Edge e : edge[x]) if (! top[e.u])
        dfs2(e.u, e.u);
}

void init (int n) {
    memset(min_edgelen, INF, sizeof min_edgelen);
    dep[1] = 1;
    dfs1 (1, -1);
    dfs2 (1, 1);
//    for (int i = 1; i <= n; i ++)
//        cout << min_edgelen[i] << " "; cout << endl;
}

int LCA (int x, int y) {
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        x = fa[top[x]];
    }
    if (dep[x] < dep[y]) swap(x, y);
    return y;
}

/// 虚树
vector<int> virtual_tree[N];
int query_arr[N];
int sta[N], sta_top;
void Insert (int x) {
    if (sta_top == 1) { sta[++ sta_top] = x; return; }
    int lca = LCA(x, sta[sta_top]);
    if (lca == sta[sta_top]) return;
    while (sta_top > 1 && dfn[ sta[sta_top - 1] ] >= dfn[lca])
        virtual_tree[ sta[sta_top - 1] ].push_back(sta[sta_top]), sta_top --;
    if (lca != sta[sta_top])
        virtual_tree[lca].push_back(sta[sta_top]), sta[sta_top] = lca;
    sta[++ sta_top] = x;
}

LL DP (int x) {
    if (! virtual_tree[x].size()) return (LL)min_edgelen[x];
    LL sum = 0;
    for (int i = 0; i < virtual_tree[x].size(); i ++)
        sum += DP(virtual_tree[x][i]);
    virtual_tree[x].clear();
    return min (sum, (LL)min_edgelen[x]);
}

int main () {
    min_edgelen[1] = INF;
    int n; scanf("%d", &n);
    for (int i = 1; i <= n; i ++) edge[i].clear();
    int u, v, w;
    for (int i = 1; i < n; i ++) {
        scanf("%d %d %d", &u, &v, &w);
        add_edge(u, v, w);
    }
    init(n);
    int query; scanf("%d", &query);
    while (query --) {
        int k; scanf("%d", &k);
        for (int i = 1; i <= k; i ++) scanf("%d", &query_arr[i]);
        sort(query_arr + 1, query_arr + 1 + k, cmp_by_dfn);
        sta[ sta_top = 1 ] = 1;
        for (int i = 1; i <= k; i ++) Insert(query_arr[i]);
        while (sta_top > 0)
            virtual_tree[sta[sta_top - 1]].push_back(sta[sta_top]), sta_top --;
        printf("%lld\n", DP(1));
    }
    return 0;
}
