/*
 * @Samaritan_infi
 */
题意：每次更新一个点子树内距离该点距离小于等于k的所有点，最后输出所有点的各个权值。
题解：离线更新，由于每一个更新只会在自己的子树里影响，所以按先序遍历顺序，更新符合深度的值，再后续遍历的下一个点减掉即可，查询即查当前点的深度值。
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

const int N = 3e5 + 5;
vector <int> edge[N];
int pre[N], post[N]; /// 先序，后序遍历
int dep[N], rev[N], dep_max;
void dfs (int now, int par) {
    static int num = 0;
    pre[now] = ++num;
    rev[num] = now;
    dep_max = max(dep_max, dep[now]);
    for (int son : edge[now]) if (son != par) {
        dep[son] = dep[now] + 1;
        dfs(son, now);
    }
    post[now] = num;
}

struct node {
    int x, y; LL val;
    node (int _x, int _y, LL _val) : x(_x), y(_y), val(_val) {}
};
vector <node> qu[N];
LL ans[N];

#define lson rt<<1
#define rson rt<<1|1
const int M = N << 2;
LL tree[M], lazy[M];
inline void push_up (int rt) {

}

inline void push_down (int rt) {
    if ((rt << 1) > M) { lazy[rt] = 0; return; }
    tree[lson] += lazy[rt]; lazy[lson] += lazy[rt];
    tree[rson] += lazy[rt]; lazy[rson] += lazy[rt];
    lazy[rt] = 0;
}

void modify (int l, int r, int rt, int L, int R, LL val) {
    if (L <= l && r <= R) {
        lazy[rt] += val;
        tree[rt] += val;
        return;
    }
    if (lazy[rt]) push_down(rt);
    int mid = (l + r) >> 1;
    if (L <= mid) modify(l, mid, lson, L, R, val);
    if (R > mid) modify(mid+1, r, rson, L, R, val);
}

LL query (int l, int r, int rt, int Q) {
    if (l == r) { return tree[rt]; }
    if (lazy[rt]) push_down(rt);
    int mid = (l + r) >> 1;
    if (Q <= mid) return query(l, mid, lson, Q);
    else return query(mid+1, r, rson, Q);
}

int main () {
    int n; scanf("%d", &n);
    int u, v;
    for (int i = 1; i < n; ++i) {
        scanf("%d %d", &u, &v);
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    dep[1] = 1; dep_max = 1;
    dfs(1, 0);
    int m; scanf("%d", &m);
    int d; LL val;
    while (m--) {
        scanf("%d %d %lld", &v, &d, &val);
        int d_ed = min (dep_max, dep[v] + d);
        qu[ pre[v] ].push_back(node(dep[v], d_ed, val));
        qu[ post[v]+1 ].push_back(node(dep[v], d_ed, -val));
    }
    for (int i = 1; i <= n; ++i) {
        for (node now : qu[i]) {
            modify(1, dep_max, 1, now.x, now.y, now.val);
        }
        ans[ rev[i] ] = query(1, dep_max, 1, dep[ rev[i] ]);
//        cout << " tree : ";
//            for (int i = 1; i <= 10; ++i) cout << tree[i] << " "; cout << endl;
    }
    for (int i = 1; i <= n; ++i) printf("%lld ", ans[i]); puts("");
    return 0;
}
