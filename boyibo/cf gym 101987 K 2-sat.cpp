/// 三个里面至少两个 == 选错一个 -> 其他两个一定正确
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
const int N = 4e5 + 5;
const int M = 1e6 + 5;
const int logN = 20;
const int INF = 0x3f3f3f3f;

struct Edge {
    int to, next;
    Edge() {}
    Edge(int _to, int _next) : to(_to), next(_next) {}
} edge[M];
int head[N], edge_cnt;
inline void add_edge(int u, int v) {
    edge[edge_cnt] = Edge(v, head[u]);
    head[u] = edge_cnt++;
}

int vis[N], belong[N];
int low[N], dfn[N], Stack[N];
int stack_num, top, scc_num;
void init(int n) { /// n point num
    memset(head, -1, sizeof (int) * (n + 2));
    edge_cnt = 1;
    /// tarjan init
    memset(dfn, 0, sizeof (int) * (n + 2));
    memset(vis, 0, sizeof (int) * (n + 2));
    scc_num = top = stack_num = 0;
}

void tarjan(int u) {
    low[u] = dfn[u] = ++ stack_num;
    Stack[top ++] = u;
    vis[u] = 1;
    for(int i = head[u]; ~ i; i = edge[i].next) {
        int v = edge[i].to;
        if(! dfn[v]) {
            tarjan(v);
            if(low[v] < low[u]) low[u] = low[v];
        } else if(vis[v] && dfn[v] < low[u]) {
            low[u] = dfn [v];
        }
    }
    if(dfn[u] == low[u]) {
        scc_num ++;
        int x;
        do {
            x = Stack[-- top];
            belong[x] = scc_num;
            vis[x] = 0;
        } while(x != u);
    }
}
/// 2-sat 建边
/// 1.如果选 A 必须 不选 B，那么建立 A -> !B 和 B -> !A 的两条边
/// 2.如果选 A 必须选 B，那么建立 A ->B 和 !B -> !A的两个边
/// 3.无论如何必须选A，建立 !A -> A 的边
bool two_sat(int n) {
    for (int i = 1; i <= n * 2; ++i)
        if (!dfn[i]) tarjan(i);
    for (int i = 1; i <= n; ++i) {
//        cout << belong[i] << " " << belong[i] << endl;
        if(belong[i] == belong[n + i]) {
            return false;
        }
    }
    return true;
}
int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    init(n * 2);
    int x[3][2]; char read[10];
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < 3; ++j) {
            scanf("%d %s", &x[j][0], read);
            if (read[0] == 'R') x[j][1] = 1;
            else x[j][1] = 0;
        }
        /// R 1 ~ n; B n + 1 ~ n + n
        for (int j = 0; j < 3; ++j) {
            int wrong = x[j][1] ^ 1;
            for (int k = 0; k < 3; ++k) if (k != j) {
                add_edge(x[j][0] + wrong * n, x[k][0] + x[k][1] * n);
                add_edge(x[k][0] + (x[k][1] ^ 1) * n, x[j][0] + (wrong ^ 1) * n);
            }
        }
    }
    if (!two_sat(n)) puts("-1");
    else {
        for (int i = 1; i <= n; ++i) {
            if (belong[i] < belong[i + n]) printf("B");
            else printf("R");
        }
        puts("");
    }
    return 0;
}
