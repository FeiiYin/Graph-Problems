/// 求森林间链接一条新的边之后，两棵子树中每个点到树上最远距离的期望
/// 枚举一颗子树的所有点， 如果链接之后整棵树的直径变长 dis[l] + dis[r] + 1 > max(d[l], d[r])
/// 后半部分的期望会改变，以此前缀和二分
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
const int mod = 1e9 + 7;
const int N = 1e5 + 5;
const int M = 1e6 + 5;
const int logN = 20;
const int INF = 1e7;

struct Edge {
    int v, nxt;
} edge[N << 1];
int head[N], edge_cnt;
inline void add_edge(int u, int v) {
    edge[edge_cnt] = {v, head[u]};
    head[u] = edge_cnt++;
}
int belong[N], belong_cnt;
/// 每一个点在自己子树里到最远点的距离
int vertice_dis[N];
/// 每一棵子树里的直径长度
int diameter[N];
/// 每一个子树里的点到最远点的距离，前缀和
vector<int> dis[N];
vector<LL> pre[N];
void init(int n) {
    memset(head, -1, sizeof(head[0]) * (n + 3));
    edge_cnt = 0;
    belong_cnt = 0;
}
/// 搜索直径
int diameter_length, st, ed;
void dfs_d(int now, int par, int belong_id, int dep) {
    belong[now] = belong_id;
    if (dep > diameter_length) {
        diameter_length = dep;
        st = now;
    }
    for (int i = head[now]; ~i; i = edge[i].nxt) {
        int to = edge[i].v;
        if (to != par) {
            dfs_d(to, now, belong_id, dep + 1);
        }
    }
}
void dfs(int now, int par, int dep) {
    vertice_dis[now] = max(vertice_dis[now], dep);
    for (int i = head[now]; ~i; i = edge[i].nxt) {
        int to = edge[i].v;
        if (to != par) {
            dfs(to, now, dep + 1);
        }
    }
}
map<PII, double> cache;

int main() {
    int n, m, q;
    scanf("%d %d %d", &n, &m, &q);
    init(n);
    int l, r;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &l, &r);
        add_edge(l, r); add_edge(r, l);
    }
    for (int i = 1; i <= n; ++i) if (!belong[i]) {
        st = i;
        diameter_length = 0;
        dfs_d(i, -1, ++belong_cnt, 0);
        ed = st;
        st = i;
        diameter_length = 0;
        dfs_d(ed, -1, belong_cnt, 0);
        diameter[belong_cnt] = diameter_length;
        dfs(st, -1, 0);
        dfs(ed, -1, 0);
    }
    for (int i = 1; i <= n; ++i) {
        dis[ belong[i] ].pb(vertice_dis[i]);
    }
    for (int i = 1; i <= belong_cnt; ++i) {
        sort (dis[i].begin(), dis[i].end());
        pre[i].resize(dis[i].size());
        pre[i][0] = dis[i][0];
        for (int j = 1; j < dis[i].size(); ++j) {
            pre[i][j] = pre[i][j - 1] + dis[i][j];
        }
    }
//    for (int i = 1; i <= belong_cnt; ++i) {
//        cout << "now : ";
//        for (int j : dis[i]) cout << j << " "; cout << endl;
//    }
    while (q--) {
        scanf("%d %d", &l, &r);
        l = belong[l]; r = belong[r];
        if (dis[l].size() > dis[r].size() || (dis[l].size() == dis[r].size() && l > r)) swap(l, r);
        if (l == r) puts("-1");
        else if (cache.count({l, r})) printf("%.10f\n", cache[{l, r}]);
        else {
            double ans = 0.0;
            int max_diameter = max(diameter[l], diameter[r]);

            for (int i : dis[l]) {
                int pos = upper_bound(dis[r].begin(), dis[r].end(), max_diameter - i - 1) - dis[r].begin();
                if (pos == dis[r].size()) ans += max_diameter * 1.0 * dis[r].size();
                else
                    ans += max_diameter * 1.0 * pos + (i + 1) * 1.0 * (dis[r].size() - pos) + pre[r].back() - (pos == 0 ? 0 : pre[r][pos - 1]);
            }
            ans /= (dis[l].size() * dis[r].size() * 1.0);
            cache[{l, r}] = ans;
            printf("%.10f\n", ans);
        }
    }
    return 0;
}
