/// 求仙人掌上的最大独立集
/// 对于树上的情况只需要 f[u][0/1] 表示 u 这个点选或不选的情况下，u 所在的子树的最大独立集。
/// 对于仙人掌，回边增加了额外的限制。考虑仙人掌对应的圆方树，需要将方点相邻的所有点的信息合并到这个环的根，枚举环的根（也就是方点的父亲）
/// 选或不选对这个环进行 dp。
#include <bits/stdc++.h>
using namespace std;
using LL = long long;
#define FOR(i, x, y) for (decay<decltype(y)>::type i = (x), _##i = (y); i < _##i; ++i)
#define FORD(i, x, y) for (decay<decltype(x)>::type i = (x), _##i = (y); i > _##i; --i)
#ifdef zerol
#define dbg(args...) do { cout << "\033[32;1m" << #args << " -> "; err(args); } while (0)
#else
#define dbg(...)
#endif
void err() { cout << "\033[39;0m" << endl; }
template<template<typename...> class T, typename t, typename... Args>
void err(T<t> a, Args... args) { for (auto x: a) cout << x << ' '; err(args...); }
template<typename T, typename... Args>
void err(T a, Args... args) { cout << a << ' '; err(args...); }
// -----------------------------------------------------------------------------
const int N = 1E5 + 100;

vector<int> G[N];
int nn;

struct E { int to, nxt; };
namespace C {
    E e[N];
    int hd[N], ecnt;
    void addedge(int u, int v) {
        e[ecnt] = {v, hd[u]};
        hd[u] = ecnt++;
    }
    int idx[N], clk, fa[N];
    bool ring[N];
    void init() { ecnt = 0; memset(hd, -1, sizeof hd); clk = 0; }
    void dfs(int u, int feid) {
        idx[u] = ++clk;
        for (int i = hd[u]; ~i; i = e[i].nxt) {
            if ((i ^ feid) == 1) continue;
            int v = e[i].to;
            if (!idx[v]) {
                fa[v] = u; ring[u] = false;
                dfs(v, i);
                if (!ring[u]) { G[u].push_back(v); G[v].push_back(u); }
            } else if (idx[v] < idx[u]) {
                ++nn;
                G[nn].push_back(v); G[v].push_back(nn); // trick!
                for (int x = u; x != v; x = fa[x]) {
                    ring[x] = true;
                    G[nn].push_back(x); G[x].push_back(nn);
                }
                ring[v] = true;
            }
        }
    }
}

int f[N][2];
int n, m;
void dfs(int u, int fa) {
    if (u <= n) {
        f[u][0] = 0; f[u][1] = 1;
        for (int& v: G[u]) {
            if (v == fa) continue;
            dfs(v, u);
            if (v > n) continue;
            f[u][0] += max(f[v][0], f[v][1]);
            f[u][1] += f[v][0];
        }
    } else {
        static int cir[N], p, g[N][2], INF = 1E9;
        for (int& v: G[u]) if (v != fa) dfs(v, u);
        p = 0; for (int& v: G[u]) cir[p++] = v;
        auto dp = [&]() {
            FOR (i, 1, p) {
                g[i][1] = f[cir[i]][1] + g[i - 1][0];
                g[i][0] = f[cir[i]][0] + max(g[i - 1][0], g[i - 1][1]);
            }
        };
        g[0][0] = f[cir[0]][0]; g[0][1] = -INF;
        dp(); int g_0 = max(g[p - 1][0], g[p - 1][1]);
        g[0][0] = -INF; g[0][1] = f[cir[0]][1];
        dp(); int g_1 = g[p - 1][0];
        f[fa][0] = g_0; f[fa][1] = g_1;
    }
}

int main() {
#ifdef zerol
    freopen("in", "r", stdin);
#endif
    cin >> n >> m; nn = n;
    C::init();
    FOR (_, 0, m) {
        int u, v; scanf("%d%d", &u, &v);
        C::addedge(u, v); C::addedge(v, u);
    }
    C::dfs(1, -1);
    dfs(1, -1);
    cout << max(f[1][0], f[1][1]) << endl;
}
