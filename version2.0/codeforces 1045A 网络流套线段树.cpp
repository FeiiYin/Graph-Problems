/*
 * @Samaritan_infi
 */
/// 题意: 有 n 个 weapon, m 个 spaceship, 求最多消灭多少个spaceship 与方案
///       weapon type : 0 k 可以选择k个中的1个消灭
///                     1 (l, r) 可以选择l, r 区间内的一个消灭
///                     2 a, b, c 选择三个中的两个或者零个消灭
/// 题解: 最大流可以解决, 但是 l, r 会建边太多, 所以在网络流建边中建一颗线段树
///       输出方案的时候, 优先选择 type 2, 如果有流量即可以选两个, 选择边与流量需要注意
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

const int maxn = 4e4 + 5, maxm = 2e5 + 5;
#define INF 100000000
struct Edge { int v, next, u, cap; }edge[maxm];
int k, head[maxn], dist[maxn], que[maxn], current[maxn];
int st, ed, POINT; /// need to be initialize
inline void dinic_init(int tot_point) {
    memset(head, -1, sizeof (head[0]) * (tot_point + 3));
    k = -1; /// very important cannot change
}
void add_edge(int u, int v, int cap) {
    edge[++k].u = u; edge[k].v = v; edge[k].cap = cap;
    edge[k].next = head[u]; head[u] = k;
    edge[++k].u = v; edge[k].v = u; edge[k].cap = 0;
    edge[k].next = head[v]; head[v] = k;
}
bool bfs() {
    int fro, top;
    memset(dist, 0, sizeof(dist[0]) * (POINT + 4));
    dist[st] = 1; fro = top = 1; que[fro] = st;
    while(fro <= top) {
        int p = que[fro++];
        for(int kk = head[p]; ~kk; kk = edge[kk].next) {
            int v = edge[kk].v, cap = edge[kk].cap;
            if(cap && !dist[v]) {
                dist[v] = dist[p] + 1; que[++top] = v;
                if(v == ed) return 1;
            }
        }
    }
    return 0;
}
int dfs(int u, int maxflow) {
    if(u == ed || maxflow == 0) return maxflow;
    int t = 0;
    for(int& kk = current[u]; ~kk; kk = edge[kk].next) {
        int v = edge[kk].v, cap = edge[kk].cap;
        if(cap && dist[v] ==  dist[u] + 1) {
            int f = dfs(v, min(cap, maxflow - t));
            t += f;
            edge[kk].cap -= f; edge[kk ^ 1].cap += f;
            if(t == maxflow) return t;
        }
    }
    if(t == 0) dist[u] = -1;
    return t;
}
int dinic() {
    int maxnflow = 0, t;
    while(bfs()) {
        for(int i = 0; i <= ed; ++i) current[i] = head[i];
        while(t = dfs(st, INF)) maxnflow += t;
    }
    return maxnflow;
}
/// segment tree
int id[maxn], rev[maxn], tot;
//void build (int nd, int l, int r) {
//    id[nd] = ++tot;
//    if (l == r) {
//        add_edge(id[nd], l, 1);
//        return;
//    }
//    int mid = l + r >> 1;
//    build(nd<<1, l, mid); build(nd<<1|1, mid+1, r);
//    add_edge(id[nd], id[nd<<1], INF);
//    add_edge(id[nd], id[nd<<1|1], INF);
//}
/// 比上面建树少一半的结点
void build (int x, int l, int r) {
    if (l == r) {
        id[l] = x; rev[x] = l;
    } else {
        int mid = l + r >> 1, z = x + (mid - l + 1 << 1);
        build(x + 1, l, mid);
        build(z, mid + 1, r);
        add_edge(x, x + 1, INF);
        add_edge(x, z, INF);
    }
}
void update (int nd, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        add_edge(tot, nd, 1); return;
    }
    int mid = l + r >> 1, z = nd + (mid - l + 1 << 1);
    /// 左边结点为 nd + 1, 右边结点为 z
    if (L <= mid) update(nd + 1, l, mid, L, R);
    if (R > mid) update(z, mid+1, r, L, R);
}
const int N = 5e3 + 5;
bool use[N]; //use_spaceship
int type[N]; // operation type
vector<int> operation[N];
inline void solve_weapon_type_2 (int &remain, int ship, int weapon) {
    --remain; use[ship] = false;
    printf("%d %d\n", weapon, ship);
}
void solve_weapon_on_spaceship (int n, int m) {
    for (int i = 1; i <= m; ++i)
        for (int j = head[id[i]]; ~j; j = edge[j].next)
            if (edge[j].v == ed && ! edge[j].cap) use[i] = true;
    int tree_point = (m << 1) - 1;
    for (int i = 1; i <= n; ++i) if (type[i] == 2) {
        /// 如果网络流中 已经流了, 一定流了2, 则先从网络流中贪心选取
        /// 如果流量是1, 一定可以从其他的流中抢一个来作为自己的使用
        bool found = false;
        for (int j = head[i + tree_point]; ~j; j = edge[j].next) {
            if (edge[j].v == st && edge[j].cap) { found = true; break; } ///这是条反边
        }
        if (found) {
            int remain = 2;
            for (int j = head[i + tree_point]; ~j; j = edge[j].next)
                if (! edge[j].cap) solve_weapon_type_2(remain, rev[edge[j].v], i);

            for (int j = 0; remain; ++j) if (use[ operation[i][j] ])
                solve_weapon_type_2 (remain, operation[i][j], i);
        }
    }
    for (int i = 1; i <= n; ++i) if (! type[i]) {
        for (int j = head[i + tree_point]; ~j; j = edge[j].next)
            if (! edge[j].cap && use[ rev[edge[j].v] ]) {
                use[ rev[edge[j].v] ] = false;
                printf("%d %d\n", i, rev[edge[j].v]);
                break;
            }
    }
    /// 剩余的给区间修改的清除
    for (int i = 1; i <= m; ++i) if (use[i]) {
        int p = -1;
        for (int j = 1; j <= n; ++j) if (type[j] == 1 && operation[j][0] <= i && operation[j][1] >= i &&
                (!~p || operation[j][1] < operation[p][1])) {
            p = j;
        }
        type[p] = -1;
        printf("%d %d\n", p, i);
    }
}
int main () {
    int n, m; scanf("%d %d", &n, &m);
    dinic_init((m << 1) - 1 + n + 2);
    build(1, 1, m);
    tot = id[m]; st = 0;
    /// for (int i = 1; i <= m; ++i) cout << id[i] << " "; cout << endl;
    /// 叶子节点的实际 id 为  id[i]
    int op, num, x, l, r;
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &type[i]); ++tot; operation[i].clear();
        if (! type[i]) {
            scanf("%d", &num);
            while (num--) {
                scanf("%d", &x); //operation[i].emplace_back(x);
                add_edge(tot, id[x], 1);
            }
            add_edge(st, tot, 1);
        } else if (type[i] == 1) {
            scanf("%d %d", &l, &r); operation[i].emplace_back(l); operation[i].emplace_back(r);
            update(1, 1, m, l, r); add_edge(st, tot, 1);
        } else {
            scanf("%d %d %d", &x, &l, &r);
            add_edge(st, tot, 2);
            operation[i].emplace_back(x); operation[i].emplace_back(l); operation[i].emplace_back(r);
            add_edge(tot, id[x], 1); add_edge(tot, id[l], 1); add_edge(tot, id[r], 1);
        }
    }
    ed = ++tot; POINT = ed + 1;
    for (int i = 1; i <= m; i++) add_edge(id[i], ed, 1);
    printf("%d\n", dinic());
    solve_weapon_on_spaceship(n, m);
    return 0;
}
