有一张n个点m条边的图，每条边上会有一个可通过数字的边界限制[l,r][l,r][l,r]，现在有数字1~1e9要从点1出发前往点n，(很明显一条边不包括数字x，那么数字x不能经过这条边)。
/// 分治，线段树，并查集合并，启发式合并
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctime>
#include <queue>
#include <cassert>
#include <iomanip>
#include <unordered_map>
//#include <unordered_map>
using namespace std;

#define rep(i,a,b) for(int i = (a); i <= (b); ++i)
#define per(i,a,b) for(int i = (a); i >= (b); --i)
#define pb emplace_back
#define mp make_pair
#define all(x) (x).begin(), (x).end()
#define fi first
#define se second
#define SZ(x) ((int)(x).size())
#define sqr(x) (x) * (x)

typedef long long ll;
#define LL long long

typedef long double ld;
typedef pair<int,int> PII;
typedef vector<int> VI;
typedef pair<PII,int> PIII;
//const int N = 100005;
const int P = 1000000007;
const int INF = 0x3f3f3f3f;
const ll LLINF = 0x3f3f3f3f3f3f3f3f;
const double eps = 1e-8;

ll qmod(ll a,ll n,ll mod){ll ans=1;while(n){assert(ans>=0);if(n&1ll)ans=ans*a%mod;n>>=1;a=a*a%mod;}return ans%mod;}

//head

//your code goes there
const int N = 2e5 + 5;
int par[N], siz[N];
struct Edge {
    int u, v, l, r;
};
Edge edge[N];
int to_discrete[N << 1], cur = 0;
vector<int> tree[N << 2];
void update(int L, int R, int id, int l, int r, int rt) {
    if (L <= l && r <= R) {
        tree[rt].pb(id);
        return;
    }
    int mid = (l + r) >> 1;
    if (L <= mid)
        update(L, R, id, l, mid, rt<<1);
    if (R > mid)
        update(L, R, id, mid+1, r, rt<<1|1);
}

int find_par (int x) {
    if (x == par[x]) return x;
    return find_par(par[x]);
}

LL ans;
int n;
void dfs (int l, int r, int rt) {
//    cout << l << " " << r << endl;
    vector<int> recall;
    for (int edge_id : tree[rt]) {
        int u = find_par(edge[edge_id].u);
        int v = find_par(edge[edge_id].v);
        if (siz[u] > siz[v])
            swap(u, v);
        /// small merge into big
        par[u] = v;
        siz[v] += siz[u];
        recall.pb(u);
    }

    if (find_par(1) == find_par(n)) {
        ans += to_discrete[r + 1] - to_discrete[l];
        //, cout << to_discrete[r + 1] << " " << to_discrete[l] << endl;
        // cout << l << ", " << r << endl;

    }
    else if (l < r) {
        int mid = (l + r) >> 1;
        dfs(l, mid, rt<<1);
        dfs(mid+1, r, rt<<1|1);
    }
    /// recall
    for (int id : recall) {
        par[id] = id;
    }
    recall.clear();
}
int main() {
    int m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i <= n; ++i) {
        par[i] = i;
        siz[i] = 1;
    }
    for (int i = 1; i <= m; ++i) {
        scanf("%d %d %d %d", &edge[i].u, &edge[i].v, &edge[i].l, &edge[i].r);
        to_discrete[++cur] = edge[i].l;
        to_discrete[++cur] = edge[i].r + 1;
    }
    sort (to_discrete + 1, to_discrete + 1 + cur);
    cur = unique(to_discrete + 1, to_discrete + 1 + cur) - (to_discrete + 1);
    for (int i = 1; i <= m; ++i) {
        int l = lower_bound(to_discrete + 1, to_discrete + 1 + cur, edge[i].l) - to_discrete;
        int r = lower_bound(to_discrete + 1, to_discrete + 1 + cur, edge[i].r + 1) - to_discrete - 1;
        update(l, r, i, 1, cur, 1);
    }
    ans = 0LL;
    dfs(1, cur, 1);
    printf("%lld\n", ans);
    return 0;
}
