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
struct node {
    int dist, id;
    node() {
    }
    node(int d, int i) : dist(d), id(i) {}
    bool operator < (const node& k) const {
        if (dist == k.dist)
            return id <= k.id;
        return dist < k.dist;
    }
};
 
const int mod = 998244353;
const int N = 1e5 + 5;
const int logN = 30;
const int INF = 0x3f3f3f3f;
 
//vector<PII> edge[N];
struct Edge {
    int v, w, nxt;
    Edge() {}
    Edge(int _v, int _w, int _nxt)
    : v(_v), w(_w), nxt(_nxt) {}
} edge[N << 1];
int head[N], edge_cnt;
inline void add_edge(int u, int v, int w) {
    edge[edge_cnt++] = Edge(v, w, head[u]);
    head[u] = edge_cnt - 1;
}
int siz[N], son_max_siz[N];
bool vis[N];
int dfs_rank[N], dfs_cnt;
 
void dfs_root(int now, int par) {
    dfs_rank[dfs_cnt++] = now;
    siz[now] = 1;
    son_max_siz[now] = 0;
//    for (const auto& p : edge[now]) {
    for (int i = head[now]; ~i; i = edge[i].nxt) {
        int to = edge[i].v;
        if (to == par || vis[to])
            continue;
        dfs_root(to, now);
        siz[now] += siz[to];
        son_max_siz[now] = max(son_max_siz[now], siz[to]);
    }
}
 
int get_root(int now) {
    dfs_cnt = 0;
    dfs_root(now, -1);
    int root_siz = INF;
    int root = -1;
    for (int i = 0; i < dfs_cnt; ++i) {
        int now_siz = max(dfs_cnt - siz[dfs_rank[i]], son_max_siz[dfs_rank[i]]);
        if (now_siz < root_siz) {
            root_siz = now_siz;
            root = dfs_rank[i];
        }
    }
    return root;
}
 
/// mul10[i] = 10^i % m
/// inv10[i] = 10^(-i) % m
LL mul10[N], inv10[N];
int n, m;
LL ans;
void init() {
    ans = 0;
    edge_cnt = 0;
    mul10[0] = inv10[0] = 1;
    LL inv = get_inv(10, m);
    for (int i = 0; i <= n; ++i) {
        head[i] = -1;
    }
    for (int i = 1; i <= n; ++i) {
        mul10[i] = mul10[i - 1] * 10 % m;
        inv10[i] = inv10[i - 1] * inv % m;
    }
}
 
 
/// num[i] = j  num(%m=i) = j
unordered_map<int, int> num;
 
LL dfs_cal1(int now, int par, int dis, LL val) {
    LL rev_val = (m + m - val * inv10[dis] % m) % m;// (m - val);
    LL res = 0;
    if (num.count(rev_val))
        res = num[rev_val];
    for (int i = head[now]; ~i; i = edge[i].nxt) {
//    for (const auto& p : edge[now]) {
        int to = edge[i].v;
        if (to == par || vis[to]) continue;
        LL now_val = (val * 10 + edge[i].w) % m;
        res += dfs_cal1(to, now, dis + 1, now_val);
    }
    return res;
}
void dfs_update1(int now, int par, int dis, LL val) {
    /// par here should not be -1
    num[val]++;
//    cout << "edge: " << now << " " << par << " val: " << val << endl;
//    for (const auto& p : edge[now]) {
    for (int i = head[now]; ~i; i = edge[i].nxt) {
        int to = edge[i].v;
        if (to == par || vis[to]) continue;
        LL now_val = (val + edge[i].w * mul10[dis]) % m;
        dfs_update1(to, now, dis + 1, now_val);
    }
}
LL dfs_cal2(int now, int par, LL val) {
    LL res = 0;
    if (par != -1 && !val) res++;
//    for (const auto& p : edge[now]) {
    for (int i = head[now]; ~i; i = edge[i].nxt) {
        int to = edge[i].v;
        if (to == par || vis[to]) continue;
        LL now_val = (val * 10 + edge[i].w) % m;
        res += dfs_cal2(to, now, now_val);
    }
    return res;
}
int temp_edge_id[N], temp_edge_id_cnt;
LL cal(int x) {
    LL res = 0;
    num.clear();
    temp_edge_id_cnt = 0;
    for (int i = head[x]; ~i; i = edge[i].nxt) {
        temp_edge_id[temp_edge_id_cnt++] = i;
//    for (const auto& p : edge[x]) {
        int to = edge[i].v;
        if (vis[to]) continue;
        int w = edge[i].w % m;
        res += dfs_cal1(to, x, 1, w);
        dfs_update1(to, x, 1, w);
    }
    num.clear();
    for (int i = temp_edge_id_cnt - 1; i >= 0; --i) {
        int to = edge[ temp_edge_id[i] ].v;
        if (vis[to]) continue;
        int w = edge[ temp_edge_id[i] ].w % m;
        res += dfs_cal1(to, x, 1, w);
        dfs_update1(to, x, 1, w);
    }
    res += num[0];
    res += dfs_cal2(x, -1, 0);
    return res;
}
 
 
void work(int x) {
    int root = get_root(x);
    ans += cal(root);
//    cout << "root: " << root << " " << ans << endl;
    vis[root] = true;
    for (int i = head[root]; ~i; i = edge[i].nxt) {
//    for (const auto& p : edge[root]) {
        int to = edge[i].v;
        if (vis[to]) continue;
        work(to);
    }
}
 
int main() {
    scanf("%d %d", &n, &m);
    init();
    int l, r, w;
    for (int i = 1; i < n; ++i) {
        scanf("%d %d %d", &l, &r, &w);
        add_edge(l, r, w);
        add_edge(r, l, w);
    }
    work(0);
    printf("%lld\n", ans);
    return 0;
}
