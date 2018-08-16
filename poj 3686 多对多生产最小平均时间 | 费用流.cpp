/*
 * @Samaritan_infi
 */
/// 白书 p 243
//#include<bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
typedef long long ll;

inline int read()
{
    int x=0,t=1;char ch=getchar();
    while((ch<'0'||ch>'9')&&ch!='-')ch=getchar();
    if(ch=='-')t=-1,ch=getchar();
    while(ch<='9'&&ch>='0')x=x*10+ch-48,ch=getchar();
    return x*t;
}

const int maxn = 1e5 + 5;
const int maxm = maxn << 3;
const int mod = 998244353;

#define type int
const int INF = 0x3f3f3f3f;
//s源点t汇点N总点数
int s, t;
struct node {
    int u, v, next;
    type cap, flow, cost;
}edge[maxm];
int head[maxn], cnt;
int pre[maxn];
type dis[maxn];
bool vis[maxn];
int N;

void init () {
    memset (head, -1, sizeof head);
    cnt = 0;
}

void add_edge (int u, int v, type cap, type cost) {
    edge[cnt].u = u, edge[cnt].v = v, edge[cnt].cap = cap, edge[cnt].flow = 0;
    edge[cnt].cost = cost, edge[cnt].next = head[u], head[u] = cnt++;
    edge[cnt].u = v, edge[cnt].v = u, edge[cnt].cap = 0, edge[cnt].flow = 0;
    edge[cnt].cost = -cost, edge[cnt].next = head[v], head[v] = cnt++;
}

bool spfa (int s, int t) {
    queue <int> q;
    for (int i = 0; i < N; i++) {
        dis[i] = INF;
        vis[i] = 0;
        pre[i] = -1;
    }
    dis[s] = 0;
    vis[s] = 1;
    q.push (s);
    while (!q.empty ()) {
        int u = q.front (); q.pop ();
        vis[u] = 0;
        for (int i = head[u]; i != -1; i = edge[i].next) {
            int v = edge[i].v;
            if (edge[i].cap > edge[i].flow && dis[v] > dis[u]+edge[i].cost) {
                dis[v] = dis[u]+edge[i].cost;
                pre[v] = i;
                if (!vis[v]) {
                    vis[v] = 1;
                    q.push (v);
                }
            }
        }
    }
    if (pre[t] == -1)
        return 0;
    else
        return 1;
}

int MCMF (int s, int t, type &cost) {
    type flow = 0;
    cost = 0;
    while (spfa (s, t)) {
        type Min = INF;
        for (int i = pre[t]; i != -1; i = pre[edge[i^1].v]) {
            if (Min > edge[i].cap-edge[i].flow) {
                Min = edge[i].cap-edge[i].flow;
            }
        }
        for (int i = pre[t]; i != -1; i = pre[edge[i^1].v]) {
            edge[i].flow += Min;
            edge[i^1].flow -= Min;
            cost += edge[i].cost*Min;
        }
        flow += Min;
    }
    return flow;
}

int pic[100][100];

void solve(int toy, int company) {
    /// 0 - n - 1 toy
    /// n - 2n - 1 company 1
    /// ....
    /// m*n - (m+1)*n - 1 company m
    s = (company + 1) * toy, t = s + 1;
    N = t + 1;
    for(int i = 0; i < toy; i ++)
        add_edge(s, i, 1, 0);
    for(int j = 0; j < company; j ++)
        for(int k = 0; k < toy; k ++) {
            add_edge(toy + j * toy + k, t, 1, 0);
            for(int i = 0; i < toy; i ++)
                add_edge(i, toy + j * toy + k, 1, (k + 1) * pic[i][j]);
        }
    int cost = toy;
    MCMF(s, t, cost);
    printf("%.6f\n", (double) cost / toy);
}

int main() {
    int kase = read();
    while(kase --) {
        init();
        int toy = read(), company = read();
        for(int i = 0; i < toy; i ++) {
            for(int j = 0; j < company; j ++)
                scanf("%d", &pic[i][j]);
        }
        solve(toy, company);
    }
    return 0;
}
