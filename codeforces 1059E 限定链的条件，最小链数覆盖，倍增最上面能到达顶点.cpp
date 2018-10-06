/*
 * @Samaritan_infi
 */
/// 链接: http://codeforces.com/problemset/problem/1059/E
/// 限定链的条件，最小链数覆盖，倍增最上面能到达顶点.
/// 题意: 每一条直链，由叶子到根，点数和权值限制，问最少多少条能够覆盖整颗树
/// 题解: 倍增，获得每个顶点最上面能到达的顶点，
///       然后优先队列维护，注意需要两个vis数组，向上更新时同时更新父亲数组
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e5 + 5;

int par[N], ancestor[N][18], top[N], in[N];
LL val[N], ancestor_val[N][18];
vector<int> edge[N];
int dep[N];
void dfs(int x) {
    for(int to : edge[x]){
        dep[to] = dep[x] + 1;
        dfs(to);
    }
}
void work_on_top (int n, int L, LL S) {
    /// top[i] 表示 i 节点作为 sequence 尾节点时最远可以向上延伸到的节点
    dep[1] = 1; dfs(1);

    for(int i = 1; i <= n; i++) {
        ancestor[i][0] = par[i];
        ancestor_val[i][0] = val[par[i]];
    }

    for(int j = 1; j < 18; j++) //倍增
        for(int i = 1; i <= n; i++)  {
            if(i + (1 << j) > n) break;
            ancestor[i][j] = ancestor[ ancestor[i][j-1] ][j-1];
            ancestor_val[i][j] = ancestor_val[i][j-1] + ancestor_val[ ancestor[i][j-1] ][j-1];
        }

    for(int i = 1; i <= n; i++) {
        LL num_limit = L - 1, val_limit = S - val[i];

        int cur = 17, x = i;
        while(cur >= 0){
            if(ancestor[x][cur] == 0 || (1 << cur) > num_limit || ancestor_val[x][cur] > val_limit) cur--;
            else {
                val_limit -= ancestor_val[x][cur];
                num_limit -= (1 << cur);
                x = ancestor[x][cur];
            }
        }
        top[i] = x;
    }
}
bool inque[N], vis[N];
priority_queue< pair<int, int> > pq;
void work (int n, int L, LL S) {
    /// 保证每次选取的链的起点的深度最大
    for(int i = 1; i <= n; i++) if(! in[i]) {
        pq.push(make_pair(dep[i], i));
        inque[i] = true;
    }
    int ans = 0, cur;
    while (pq.size()) {
        int u = pq.top().second; pq.pop();
        if(vis[u]) continue;

        int top_now = top[u], now = u, far = par[u];
        while (dep[far] > dep[top_now]) {
            vis[now] = true;
            par[now] = top_now;
            now = far;
            far = par[now];
        }
        vis[now] = 1;
        vis[top_now] = 1;
        if(now != top_now) par[now] = top_now;

        ans++;
        if (par[top_now] && ! vis[par[top_now]] && !inque[par[top_now]]) {
            pq.push(make_pair(dep[par[top_now]], par[top_now]));
            inque[par[top_now]]=1;
        }
    }
    printf("%d\n",ans);
}
int main() {
    int n, L;
    LL S, max_val = 0;
    scanf("%d %d %lld", &n, &L, &S);
    for(int i = 1; i <= n; i++) {
        scanf("%d", &val[i]);
        max_val = max(max_val, val[i]);
    }
    par[1] = 0;
    for(int i = 2; i <= n; i++) {
        scanf("%d", &par[i]), in[par[i]]++;
        edge[par[i]].push_back(i);
    }
    if(max_val > S) { puts("-1"); return 0; }
    work_on_top(n, L, S);
    work(n, L, S);
    return 0;
}
