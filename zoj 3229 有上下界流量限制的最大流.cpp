/*
 * @Samaritan_infi
 */
///  要求:
/// m girls
/// girl i :  every day :[Li, Ri]
/// sum girl i >= Gi
/// every day sum <= Di

#include<bits/stdc++.h>
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

const int maxm = 1e6 + 5;
const int maxn = 2e4 + 5;
const int DAY = 510, GIRL = 1e3 + 15;
const int inf = 2147483640;
const int oo = 1e8;

struct Edge {
    int v, next, u, cap;
}edge[maxm];
int k, head[maxn], dist[maxn], que[maxn], n, m, st, ed, current[maxn];

void init() {
    memset(head, -1, sizeof head);
    k = -1; /// very important cannot change
}

void add_edge(int u, int v, int cap) {
    edge[++k].u = u;
    edge[k].v = v;
    edge[k].cap = cap;
    edge[k].next = head[u];
    head[u] = k;
    edge[++k].u = v;
    edge[k].v = u;
    edge[k].cap = 0;
    edge[k].next = head[v];
    head[v] = k;
}
bool bfs() {
    int fro, top;
    memset(dist, 0, sizeof(dist));
    dist[st] = 1;
    fro = top =1;
    que[fro] = st;
    while(fro <= top)
    {
        int p = que[fro++];
        for(int kk = head[p]; ~kk; kk = edge[kk].next)
        {
            int v = edge[kk].v, cap = edge[kk].cap;
            if(cap && !dist[v])
            {
                dist[v] = dist[p] + 1;
                que[++top] = v;
                if(v == ed) return 1;
            }
        }
    }
    return 0;
}
int dfs(int u, int maxflow) {
    if(u == ed || maxflow == 0) return maxflow;
    int t = 0;
    for(int& kk = current[u]; ~kk; kk = edge[kk].next)
    {
        int v =  edge[kk].v, cap = edge[kk].cap;
        if(cap && dist[v] ==  dist[u] + 1)
        {
            int f = dfs(v, min(cap, maxflow - t));
            t += f;
            edge[kk].cap -= f;
            edge[kk ^ 1].cap += f;
            if(t == maxflow) return t;
        }
    }
    if(t == 0) dist[u] = -1;
    return t;
}
int dinic() {
    int maxnflow = 0, t;
    while(bfs()) {
        for(int i = 0; i <= ed; ++i) current[i] = head[i];
        while(t = dfs(st, inf))
            maxnflow += t;
    }
    return maxnflow;
}

int degree[DAY + GIRL + 105];
int low_bound[DAY][GIRL], output_edge_id[DAY][GIRL];

int main() {
    int n, m;
    while(~ scanf("%d %d", &n, &m)) {
        init();
        memset(degree, 0, sizeof (int) * (n + m + 100));
        memset(output_edge_id, 0, sizeof output_edge_id);

        st = 0, ed = n + m + 1;
        int x;
        for(int i = 1; i <= m; i ++) {
            scanf("%d", &x);
            /// 维护边下界值
            /// 汇入加，流出减
            degree[ed] += x;
            degree[i + n] -= x;
            add_edge(i + n, ed, oo - x);
        }

        for(int i = 1, op, di; i <= n; i ++) {
            scanf("%d %d", &op, &di);
            add_edge(st, i, di);
            for(int j = 1, gi, l, r; j <= op; j ++) {
                scanf("%d %d %d", &gi, &l, &r);
                gi ++;
                degree[i] -= l;
                degree[gi + n] += l;
                add_edge(i, gi + n, r - l);

                low_bound[i][gi] = l;
                output_edge_id[i][gi] = k;
            }
        }
        /// 构造无汇源的上下界图
        add_edge(ed, st, oo);
        /// 第一遍最大流检验是否可行
        /// 构造超级源点，超级汇点
        st = n + m + 2, ed = st + 1;
        head[st] = head[ed] = -1;

        int sum = 0;
        for(int i = 0; i <= ed - 2; i ++) {
            if(degree[i] > 0)
                sum += degree[i], add_edge(st, i, degree[i]);
            else
                add_edge(i, ed, - degree[i]);
        }
        int maxFlow = dinic();
        if(maxFlow != sum) {
            //cout << maxFlow << " " << sum << endl;
            puts("-1");
            puts("");
            continue;
        }

        /// 删掉超级源点，超级汇点
        /// 从新对st，ed进行最大流，此时跑一边即为maxflow
        /// 剩下的残量网络 + 第一次下界流满的流
        head[st] = head[ed] = -1;
        st = 0, ed = n + m + 1;
        maxFlow = dinic();
        printf("%d\n", maxFlow);
        for(int i = 1; i <= n; i ++) {
            for(int j = 1; j <= m; j ++)
            /// 输出当前边的流量，即反向边的剩余流量
                if(output_edge_id[i][j])
                    printf("%d\n", edge[ output_edge_id[i][j] ].cap + low_bound[i][j]);
        }
        puts("");
    }

    return 0;
}

/*
/// 最小流求法
   1、du[i]表示i节点的入流之和与出流之和的差。 
   2、增设超级源点st和超级汇点sd，连（st，du[i]（为正）），（-du[i]（为负），sd）。 
   ///增设超级源点和超级汇点，因为网络中规定不能有弧指向st，也不能有流量流出sd
   3、做一次maxflow（）。
   4、源点（Sd）和起点（St）连一条容量为oo的边。
   5、再做一次maxflow（）。
   6、当且仅当所有附加弧满载时有可行流，最后答案为flow[（Sd->St）^1]，St到Sd最大流就是Sd到St最小流。
*/
