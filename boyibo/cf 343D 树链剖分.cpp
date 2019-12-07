///
/**
 * @Samaritan
 */
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;

#define pb emplace_back
#define fi first
#define se second
#define PII pair<int, int>
#define PLL pair<LL, LL>
#define PLI pair<LL, int>

LL quick_pow (LL a, LL b, LL mod) {
    LL ans = 1, base = a;
    while (b) {
        if (b & 1) (ans *= base) %= mod;
        b >>= 1;
        (base *= base) %= mod;
    }
    return ans;
}
void gcd(LL a, LL b, LL &d, LL &x, LL &y) {
    if (!b) {
        d = a; x = 1; y = 0;
    } else {
        gcd(b, a % b, d, y, x);
        y -= x * (a / b);
    }
}
LL get_inv(LL a, LL n) {
    LL d, x, y;
    gcd(a, n, d, x, y);
    return d == 1 ? (x + n) % n : -1;
}
inline int log2(const float &x) {
    return ((unsigned &) x >> 23 & 255) - 127;
}
template <typename type>
void show(type arr[], int len) {
    for (int i = 0; i <= len; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
const int mod = 998244353;
const int N = 5e5 + 5;
const int logN = 20;
const int INF = 0x3f3f3f3f;

struct Edge {
    int v, nxt;
} edge[N << 1];
int head[N], edge_cnt;
inline void add_edge(int l, int r) {
    edge[edge_cnt] = {r, head[l]};
    head[l] = edge_cnt++;
}
/// 树链剖分
int siz[N], son[N], top[N], dep[N], far[N];
int id[N], id_cnt;
void dfs1(int now, int par, int d) {
    far[now] = par; siz[now] = 1; dep[now] = d; son[now] = 0;
    for (int i = head[now]; ~i; i = edge[i].nxt) {
        int to = edge[i].v;
        if (to == par) continue;
        dfs1(to, now, d + 1);
        siz[now] += siz[to];
        if (!son[now] || siz[to] > siz[son[now]]) {
            son[now] = to;
        }
    }
}
void dfs2(int now, int par, int tp) {
    top[now] = tp; id[now] = ++id_cnt;
    if (son[now]) dfs2(son[now], now, tp);
    for (int i = head[now]; ~i; i = edge[i].nxt) {
        int to = edge[i].v;
        if (to == par || to == son[now]) continue;
        dfs2(to, now, to);
    }
}
void init(int n) {
    edge_cnt = 0;
    id_cnt = 0;
    memset(head, -1, sizeof (int) * (n + 3));
}
/// segment tree
#define lson rt<<1
#define rson rt<<1|1
struct node { int l, r, tag, lazy; } tree[N << 2];
inline void push_down(int rt) {
    if (tree[rt].lazy != -1) {
        tree[lson].tag = tree[lson].lazy = tree[rson].tag = tree[rson].lazy = tree[rt].lazy;
        tree[rt].lazy = -1;
    }
}
void build (int l, int r, int rt) {
    tree[rt] = {l, r, 0, -1};
    if (l == r) {
        return;
    }
    int mid = (l + r) >> 1;
    build(l, mid, lson);
    build(mid+1, r, rson);
}
void update(int rt, int L, int R, int val) {
    if (L <= tree[rt].l && tree[rt].r <= R) {
        tree[rt].tag = tree[rt].lazy = val;
        return;
    }
    push_down(rt);
    int mid = (tree[rt].l + tree[rt].r) >> 1;
    if (L <= mid) update(lson, L, R, val);
    if (R > mid) update(rson, L, R, val);
}
int query(int rt, int pos) {
    if (tree[rt].l == tree[rt].r) return tree[rt].tag;
    push_down(rt);
    int mid = (tree[rt].l + tree[rt].r) >> 1;
    if (pos <= mid) return query(lson, pos);
    else return query(rson, pos);
}
void update_chain(int u, int v, int val) {
    int f1 = top[u], f2 = top[v];
    while (f1 != f2) {
        if (dep[f1] < dep[f2]) {
            swap(f1, f2); swap(u, v);
        }
        update(1, id[f1], id[u], val);
        u = far[f1]; f1 = top[u];
    }
    if (dep[u] > dep[v])
        swap(u, v);
    update(1, id[u], id[v], val);
}
int main() {
    int n; scanf("%d", &n);
    init(n);
    int l, r;
    for (int i = 1; i < n; ++i) {
        scanf("%d %d", &l, &r);
        add_edge(l, r);
        add_edge(r, l);
    }
    dfs1(1, -1, 1);
    dfs2(1, -1, 1);
    build(1, id_cnt, 1);
    int q, op, pos; scanf("%d", &q);
    while (q--) {
        scanf("%d %d", &op, &pos);
        if (op == 1) update(1, id[pos], id[pos] + siz[pos] - 1, 1);
        else if (op == 2) update_chain(1, pos, 0);
        else if (op == 3) printf("%d\n", query(1, id[pos]));
    }
    return 0;
}
