/*
 * @Samaritan_infi
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N = 1e6 + 5;
vector<int> edge[N];
int degree[N], label[N];

inline void init_label (int now) {
    label[now] = 1;
    for(int to : edge[now]) {
        if(label[to]) {
            degree[to] ++;
        } else {
            label[to] = 2;
        }
    }
}

bool vis[N];
vector<int> output;
void dfs (int now) {
    if(label[now] != 1 || degree[now] != 0) return;
    degree[now] = -1;
    if(! vis[now])
        output.emplace_back(now);
    for(int to : edge[now]) {
        if(! vis[now]) vis[to] = true;
        degree[to] --;
        dfs(to);
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int l, r;
    for(int i = 0; i < m; i ++) {
        scanf("%d %d", &l, &r);
        l --, r --;
        edge[l].emplace_back(r);
    }
    for(int i = 0; i < n; i ++)  if(! label[i])
        init_label(i);

    //for(int i = 0; i < n; i ++) cout << label[i] << " "; cout << endl;
    for(int i = 0; i < n; i ++) dfs(i);

    printf("%d\n", output.size());
    for(int i : output) printf("%d ", i + 1);
    puts("");
    return 0;
}
