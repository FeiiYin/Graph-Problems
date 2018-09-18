/*
 * @Samaritan_INFi
 */
#include <bits/stdc++.h>
#pragma comment(linker, "/STACK:102400000,102400000")
using namespace std;
typedef long long LL;

const int MAXN = 8010;
const int MAXM = 1000010;
const int INF = 0x3f3f3f3f;
const double eps = 1e-8;

struct Edge {
    int to, next;
    double cap, flow;
} edge[MAXM];

int tol;
int Head[MAXN];
int gap[MAXN], dep[MAXN], cur[MAXN];

void init() {
    tol = 0;
    memset(Head, -1, sizeof(Head));
}

void addedge(int u, int v, double w, double rw = 0) {
    edge[tol].to = v;
    edge[tol].cap = w;
    edge[tol].flow = 0;
    edge[tol].next = Head[u];
    Head[u] = tol++;
    edge[tol].to = u;
    edge[tol].cap = rw;
    edge[tol].flow = 0;
    edge[tol].next = Head[v];
    Head[v] = tol++;
}

int Q[MAXN];

void BFS(int start, int end) {
    memset(dep, -1, sizeof(dep));
    memset(gap, 0, sizeof(gap));
    gap[0] = 1;
    int front = 0, rear = 0;
    dep[end] = 0;
    Q[rear++] = end;
    while (front != rear)
    {
        int u = Q[front++];
        for (int i = Head[u]; i != -1; i = edge[i].next)
        {
            int v = edge[i].to;
            if (dep[v] != -1)continue;
            Q[rear++] = v;
            dep[v] = dep[u] + 1;
            gap[dep[v]]++;
        }
    }
}

int S[MAXN];

double sap(int start, int end, int N) {
    BFS(start, end);
    memcpy(cur, Head, sizeof(Head));
    int top = 0;
    int u = start;
    double ans = 0;
    while (dep[start] < N)
    {
        if (u == end)
        {
            double Min = INF;
            int inser;
            for (int i = 0; i < top; i++)
                if (Min > edge[S[i]].cap - edge[S[i]].flow)
                {
                    Min = edge[S[i]].cap - edge[S[i]].flow;
                    inser = i;
                }
            for (int i = 0; i < top; i++)
            {
                edge[S[i]].flow += Min;
                edge[S[i] ^ 1].flow -= Min;
            }
            ans += Min;
            top = inser;
            u = edge[S[top] ^ 1].to;
            continue;
        }
        bool flag = false;
        int v;
        for (int i = cur[u]; i != -1; i = edge[i].next)
        {
            v = edge[i].to;
            if (edge[i].cap - edge[i].flow && dep[v] + 1 == dep[u])
            {
                flag = true;
                cur[u] = i;
                break;
            }
        }
        if (flag)
        {
            S[top++] = cur[u];
            u = v;
            continue;
        }
        int Min = N;

        for (int i = Head[u]; i != -1; i = edge[i].next)
            if (edge[i].cap - edge[i].flow && dep[edge[i].to] < Min)
            {
                Min = dep[edge[i].to];
                cur[u] = i;
            }
        gap[dep[u]]--;
        if (!gap[dep[u]])return ans;
        dep[u] = Min + 1;
        gap[dep[u]]++;
        if (u != start)u = edge[S[--top] ^ 1].to;
    }
    return ans;
}

struct node {
    int u, v;
} v[MAXM];

int n, m;
int a[MAXN];

bool ok(double mid)
{
    init();
    int s = 0, t = n + m + 1;
    for (int i = 1; i <= m; i++) addedge(s, i, 1);
    for (int i = 1; i <= n; i++) addedge(i + m, t, mid);

    for (int i = 1; i <= m; i++)
    {
        addedge(i, v[i].u + m, INF);
        addedge(i, v[i].v + m, INF);
    }

    //printf("%d %d %d %d\n", s, t, n, m);
    double res = sap(s, t, t + 1);
    return 1.0 * m - res < 1e-6;
}

int d[MAXN];

bool solve(double mid)
{
    init();

    int s = 0, t = n + 1;
    for (int i = 1; i <= m; i++) {
        addedge(v[i].v, v[i].u, 1);
        addedge(v[i].u, v[i].v, 1);
    }
    for (int i = 1; i <= n; i++) {
        addedge(s, i, m);
        addedge(i, t, m + 2 * mid - d[i]);
    }
    double res = sap(s, t, t + 1);
    return 1.0 * m * n - res >= eps;
}
int main() {
    int tt;
    cin >> tt;
    for (int ca = 1; ca <= tt; ca++)
    {
        printf("Case #%d: ", ca);
        m = 1;

        memset(d, 0, sizeof(d));
        cin >> n;
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        for (int i = 1; i <= n; i++)
            for (int j = i + 1; j <= n; j++)
            {
                if (a[i] > a[j])
                {
                    v[m].u = i, v[m].v = j;
                    d[v[m].u]++, d[v[m].v]++;
                    m++;
                }
            }
        m--;

        double l = 0, r = m;
        while (r - l > 1e-7)
        {
            double mid = (l + r) / 2;
            if (ok(mid))
                r = mid;
            else
                l = mid;
        }

        // double l = 0, r = m;
        // while (r - l > 1e-7)
        // {
        //     double mid = (l + r) / 2;
        //     if (solve(mid))
        //         l = mid;
        //     else
        //         r = mid;
        // }
        
        printf("%.12f\n", l + eps);
    }
    return 0;
}
