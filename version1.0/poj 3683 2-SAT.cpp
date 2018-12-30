/*
 * @Samaritan_infi
 */
//#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
using namespace std;
typedef long long ll;

const int N = 2e3 + 5;

vector<int> edge[N];
vector<int> reverse_edge[N];
vector<int> vs;
/// 记录属于第几个联通块
int belong[N];
bool vis[N];

void add_edge(int l, int r) {
    edge[l].push_back(r);
    reverse_edge[r].push_back(l);
}

void dfs(int now) {
    //cout << now << endl;
    vis[now] = true;
    for(int i = 0; i < edge[now].size(); i ++) {
        int to = edge[now][i];
        if(! vis[to])
            dfs(to);
    }
    vs.push_back(now);
}

void rdfs(int now, int scc_num) {
    vis[now] = true;
    belong[now] = scc_num;
    for(int i = 0; i < reverse_edge[now].size(); i ++) {
        int to = reverse_edge[now][i];
        if(! vis[to])
            rdfs(to, scc_num);
    }
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

char read[20];
inline int into_minite() {
    int a = (read[0] - '0') * 10 + read[1] - '0';
    int b = (read[3] - '0') * 10 + read[4] - '0';
    return a * 60 + b;
}

int start[N], finish[N], interval[N];

void solve(int n) {
    /// 1 ~ n x_i
    /// n + 1 ~ 2 * n (! x_i)
    int point = n << 1;
    for(int i = 1; i <= n; i ++) {
        for(int j = 1; j < i; j ++) {
            if(min(start[i] + interval[i], start[j] + interval[j]) > max(start[i], start[j])) {
                /// xi -> !xj, xj -> !xi
                add_edge(i, n + j);
                add_edge(j, n + i);
            }
            if(min(start[i] + interval[i], finish[j]) > max(start[i], finish[j] - interval[j])) {
                /// xi -> xj, !xj -> !xi
                add_edge(i, j);
                add_edge(n + j, n + i);
            }
            if(min(finish[i], start[j] + interval[j]) > max(finish[i] - interval[i], start[j])) {
                /// !xi -> !xj, xj -> xi
                add_edge(j, i);
                add_edge(n + i, n + j);
            }
            if(min(finish[i], finish[j]) > max(finish[i] - interval[i], finish[j] - interval[j])) {
                /// !xi -> xj, !xj -> xi
                add_edge(i + n, j);
                add_edge(n + j, i);
            }
        }
    }

    int scc_num = scc(point);
    /// 判断是否满足
    for(int i = 1; i <= n; i ++) {
        if(belong[i] == belong[n + i]) {
            puts("NO");
            return;
        }
    }

    puts("YES");
    /// scc的大小即拓扑排序的结果
    for(int i = 1; i <= n; i ++) {
        if(belong[i] > belong[i + n]) {
            /// xi 为真
            printf("%02d:%02d %02d:%02d\n", start[i]/60, start[i]%60,
                   (start[i]+interval[i])/60, (start[i]+interval[i])%60);
        } else {
            /// xi 为假
            printf("%02d:%02d %02d:%02d\n", (finish[i]-interval[i])/60, (finish[i]-interval[i])%60,
                   (finish[i])/60, (finish[i])%60);
        }
    }
}

int main() {
    int n;
    while(~ scanf("%d", &n)) {
        int point = n << 1;
        for(int i = 0; i <= point; i ++) {
            edge[i].clear();
            reverse_edge[i].clear();
            belong[i] = 0;
        }
        int x;
        for(int i = 1; i <= n; i ++) {
            scanf("%s", read);
            start[i] = into_minite();
            scanf("%s", read);
            finish[i] = into_minite();
            scanf("%d", &interval[i]);
        }

        solve(n);
    }
    return 0;
}
