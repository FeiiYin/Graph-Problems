/*
 * @Samaritan_infi
 */
//#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long ll;

const int N = 2e4 + 10;
const int M = 5e4 + 10;

struct Edge {
    int to, next;
    Edge() {}
    Edge(int _t, int _n) : to(_t), next(_n) {}
} edge[M];

int head[N], vis[N], belong[N];
int low[N], dfn[N], Stack[N];
int edge_num;

void add_edge(int u, int v) {
    edge[edge_num] = Edge(v, head[u]);
    head[u] = edge_num++;
}

int stack_num, top, scc_num;
void init(int n) {
    memset(dfn, 0, sizeof (int) * (n + 2));
    memset(head, -1, sizeof (int) * (n + 2));
    memset(vis, 0, sizeof (int) * (n + 2));
    edge_num = 1;
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

int degree[N];

int main() {
    int n, m;
    while(~ scanf("%d %d", &n, &m)) {
        init(n);
        int l, r;
        for(int i = 1; i <= m; i ++) {
            scanf("%d %d", &l, &r);
            add_edge(l, r);
        }
        for(int i = 1; i <= n; i ++)
            if(! dfn[i]) tarjan(i);

        memset(degree, 0, sizeof degree);
        for(int u = 1; u <= n; u ++) {
            for(int i = head[u]; ~i; i = edge[i].next) {
                int v = edge[i].to;
                if(belong[u] != belong[v])
                    degree[ belong[u] ] ++;
            }
        }
        int ans = 0, ans_scc;
        for(int i = 1; i <= scc_num; i ++) {
            if(! degree[i]) {
                ans ++;
                ans_scc = i;
            }
        }
        if(ans == 1) {
            ans = 0;
            for(int i = 1; i <= n; i ++) {
                if(belong[i] == ans_scc)
                    ans ++;
            }
        }
        else ans = 0;
        printf("%d\n", ans);
    }
    return 0;
}
