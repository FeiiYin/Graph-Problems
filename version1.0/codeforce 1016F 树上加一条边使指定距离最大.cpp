/*
 * @Samaritan_infi
 * codeforces.com/contest/1016/problem/F

 */
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N = 3e5 + 5;
vector< pair<int, int> > edge[N];
vector<int> path;
vector<ll> dist;
bool on_path[N];
int n;

bool dfs(int now, int par, ll dis) {
    if(now == n) {
        path.emplace_back(now);
        dist.emplace_back(dis);
        return true;
    }
    for(auto next : edge[now]) {
        int to = next.first;
        int val = next.second;
        if(to == par) continue;
        if(dfs(to, now, dis + val)) {
            path.emplace_back(now);
            dist.emplace_back(dis);
            return true;
        }
    }
    return false;
}


/// 如果路径上的顶点有两个非路径的孩子，或者有一条大于等于二的孩子链
/// 都可以直接加一条边形成一个小三角形，使其不影响1到n的路径长
bool flag;

void dfs_status(int now, int par, ll dis, int & son, ll & son_dis) {
    if(now != par) {
        if(son) {
            flag = true;
            return;
        }
        son = now;
        son_dis = dis;
    }
    for(auto next : edge[now]) {
        if(on_path[next.first] || next.first == par) continue;
        dfs_status(next.first, now, next.second + dis, son, son_dis);
    }
}


int son[N];
ll son_dis[N];

int main() {
    int m;
    scanf("%d %d", &n, &m);
    for(int i = 1; i < n; i ++){
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        edge[u].emplace_back(make_pair(v, w));
        edge[v].emplace_back(make_pair(u, w));
    }
    assert(dfs(1, 1, 0));
    reverse(path.begin(), path.end());
    reverse(dist.begin(), dist.end());
    memset(on_path, 0, sizeof on_path);
    for(int i : path)
        on_path[i] = true;

    flag = false;
    for(int i : path) {
        son[i] = son_dis[i] = 0;
        dfs_status(i, i, 0, son[i], son_dis[i]);

        if(flag) break;
    }

    ll best = 1e16;

    if(! flag) {
        /// 如果说路径上的所有点都最多只有一个非路径孩子的情况下
        for(int i = 0; i < path.size(); i ++) {
            if(i > 1)
                best = min(best, dist[i] - dist[i - 2]);
            int point = path[i];
            if(son[ point ]) {
                if(i > 0)
                    best = min(best, dist[i] - dist[i - 1] - son_dis[point]);
                if(i + 1 < path.size())
                    best = min(best, dist[i + 1] - dist[i] - son_dis[point]);
                if(i > 0 && son[ path[i - 1] ])
                    best = min(best, dist[i] - dist[i - 1] - son_dis[point] - son_dis[ path[i - 1] ]);
            }
        }
    }

    ll x;
    ll tot_dis = dist.back();
    while(m --) {
        scanf("%lld", &x);
        if(flag)
            printf("%lld\n", tot_dis);
        else
            printf("%lld\n", min(tot_dis, tot_dis + x - best));
    }

    return 0;
}
