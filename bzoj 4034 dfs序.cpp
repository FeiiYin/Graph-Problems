/*
 * @Samaritan_infi
 * 操作 1 单点增加权值val
 * 操作 2 子树增加权值val
 * 操作 3 查询 x 到根的路径的权值和
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 1e5 + 5;

struct Edge {
    int to, next;
    Edge () {}
    Edge (int a, int b) : to(a), next(b) {}
};
Edge edge[N << 1];
int head[N], edge_num;
ll weight[N];

inline void add_edge (int a, int b) {
    edge[edge_num] = Edge(b, head[a]);
    head[a] = edge_num ++;
    edge[edge_num] = Edge(a, head[b]);
    head[b] = edge_num ++;
}

int inID[N], outID[N];
int inout[N << 1]; /// 1 in | -1 out
int newID[N << 1], new_pos; /// dfs sequence

void DFS(int node, int par) {
    newID[new_pos] = node;
    inout[new_pos] = 1;
    inID[node] = new_pos ++;
    for (int i = head[node]; ~ i; i = edge[i].next)
        if (edge[i].to != par)
            DFS(edge[i].to, node);
    newID[new_pos] = node;
    inout[new_pos] = -1;
    outID[node] = new_pos ++;
}

ll tree[N << 3];
ll lazy[N << 3];
int flag[N << 3]; /// count of positive number in the range

inline void push_up(int node) {
    tree[node] = tree[node << 1] + tree[node << 1 | 1];
    flag[node] = flag[node << 1] + flag[node << 1 | 1];
}

inline void push_down(int node) {
    if (! lazy[node]) return;
    ll x = lazy[node];
    tree[node << 1] += flag[node << 1] * x;
    lazy[node << 1] += x;
    tree[node << 1 | 1] += flag[node << 1 | 1] * x;
    lazy[node << 1 | 1] += x;
    lazy[node] = 0LL;
}

void build(int node, int l, int r) {
    lazy[node] = 0LL;
    if (l == r) {
        tree[node] = inout[l] * weight[ newID[l] ];
        flag[node] = inout[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(node << 1, l, mid);
    build(node << 1 | 1, mid + 1, r);
    push_up(node);
}

void modify(int node, int l, int r, int L, int R, ll val) {
    if (L <= l && r <= R) {
        tree[node] += flag[node] * val;
        lazy[node] += val;
        return;
    }
    push_down(node);
    int mid = (l + r) >> 1;
    if (L <= mid) modify(node << 1, l, mid, L, R, val);
    if (mid < R)  modify(node << 1 | 1, mid + 1, r, L, R, val);
    push_up(node);
}

ll query(int node, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tree[node];
    push_down(node);
    ll ans = 0LL;
    int mid = (l + r) >> 1;
    if (L <= mid) ans += query(node << 1, l, mid, L, R);
    if (mid < R)  ans += query(node << 1 | 1, mid + 1, r, L, R);
    return ans;
}

void init(int n) {
    edge_num = 0;
    memset(head, -1, sizeof (int) * (n + 3));
    new_pos = 1; /// important to set l,r between 1 and n << 1
}

void work(int n, int op) {
    int operation, x; ll val;
    while (op --) {
        scanf("%d %d", &operation, &x);
        if (operation == 1) {
            scanf("%lld", &val);
            modify(1, 1, n << 1, inID[x], inID[x], val);
            modify(1, 1, n << 1, outID[x], outID[x], val);
        } else if (operation == 2) {
            scanf("%lld", &val);
            modify(1, 1, n << 1, inID[x], outID[x], val);
        } else if (operation == 3) {
            printf("%lld\n", query(1, 1, n << 1, 1, inID[x]));
        }
    }
}

int main() {
    int n, op;
    while (~ scanf("%d %d", &n, &op)) {
        init(n);
        for (int i = 1; i <= n; i ++) scanf("%lld", &weight[i]);
        int u, v;
        for (int i = 1; i < n; i ++) {
            scanf("%d %d", &u, &v);
            add_edge(u, v);
        }
        DFS(1, -1);
        build(1, 1, n << 1);
        work(n, op);
    }
    return 0;
}
/*
5 5
1 2 3 4 5
1 2
1 4
2 3
2 5
3 3
1 2 1
3 5
2 1 2
3 3
*/
