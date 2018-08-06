/*
 * @Samaritan_infi
 */
//#include <bits/stdc++.h>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
typedef long long ll;

const int N = 1e4 + 5;

vector<int> edge[N];
vector<int> reverse_edge[N];
vector<int> vs;
/// 记录属于第几个联通块
int save[N];
bool vis[N];

void add_edge(int l, int r) {
    edge[l].emplace_back(r);
    reverse_edge[r].emplace_back(l);
}

void dfs(int now) {
    vis[now] = true;
    for(int to : edge[now])
        if(! vis[to])
            dfs(to);
    vs.emplace_back(now);
}

void rdfs(int now, int scc_num) {
    vis[now] = true;
    save[now] = scc_num;
    for(int to : reverse_edge[now])
        if(! vis[to])
            rdfs(to, scc_num);
}

int scc(int n) {
    memset(vis, 0, sizeof vis);
    vs.clear();
    for(int i = 1; i <= n; i ++)
        if(! vis[i]) dfs(i);
    memset(vis, 0, sizeof vis);
    int scc_num = 0;
    for(int i = vs.size() - 1; i >= 0; i --)
        if(! vis[ vs[i] ])
            rdfs(vs[i], ++ scc_num);
    return scc_num;
}

int main() {
    int n, m;
    while(~ scanf("%d %d", &n, &m)) {
        for(int i = 0; i <= n; i ++) {
            edge[i].clear();
            reverse_edge[i].clear();
            save[i] = 0;
        }
        int l, r;
        for(int i = 0; i < m; i ++) {
            scanf("%d %d", &l, &r);
            add_edge(l, r);
        }

        int scc_num = scc(n);
        int num = 0;
        int start;
        /// 转为DAG拓扑排序的末端是scc标号最大的
        for(int i = 1; i <= n; i ++)
            if(save[i] == scc_num) {
                num ++;
                start = i;
            }
        memset(vis, 0, sizeof vis);
        rdfs(start, 1);
        for(int i = 1; i <= n; i ++)
            if(! vis[i])
                num = 0;
        printf("%d\n", num);
    }
    return 0;
}
