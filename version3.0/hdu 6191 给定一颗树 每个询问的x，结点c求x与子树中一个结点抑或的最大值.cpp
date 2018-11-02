/*
 * @Samaritan_infi
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e5 + 5;
int val[N], far[N];

int trie[N * 40][2], son[N * 40][2], root[N], trie_num;

void build (int last, int cur, int num, int pos) {
    if(pos < 0) return;
    int temp = (num & (1 << pos)) > 0;
    trie[cur][temp] = trie[last][temp] + 1;
    trie[cur][temp^1] = trie[last][temp^1];
    son[cur][temp] = ++trie_num;
    son[cur][temp^1] = son[last][temp^1]; /// 另一个孩子没变
    build(son[last][temp], son[cur][temp], num, pos-1);
}
int dfs_fir[N], dfs_las[N], dfs_num;
vector <int> edge[N];
void dfs (int node) {
    dfs_fir[node] = ++dfs_num;
    root[dfs_fir[node]] = ++trie_num;
    build(root[dfs_fir[node]-1], root[dfs_fir[node]], val[node], 30);
    for (int son : edge[node]) dfs(son);
    dfs_las[node] = dfs_num;
}
int query (int last, int cur, int num, int sum, int pos) {
    if(pos < 0) return sum;
    int temp = !!(num & (1<<pos));
    if(trie[cur][temp^1]-trie[last][temp^1]>0)
        return query(son[last][temp^1],son[cur][temp^1],num,sum|(1<<pos),pos-1);
    else
        return query(son[last][temp],son[cur][temp],num,sum,pos-1);
}
int main () {
    int n, q;
    while (~ scanf("%d %d", &n, &q)) {
        trie_num = dfs_num = 0;
        memset(trie, 0, sizeof trie);
        memset(son, 0, sizeof son);
        memset(root, 0, sizeof root);
        for (int i = 1; i <= n; ++i) edge[i].clear();
        for (int i = 1; i <= n; ++i) scanf("%d", &val[i]);
        for (int i = 2; i <= n; ++i) {
            scanf("%d", &far[i]);
            edge[ far[i] ].push_back(i);
        }
        dfs (1);
        int u, x;
        while (q--) {
            scanf("%d %d", &u, &x);
            printf("%d\n", query(root[dfs_fir[u]-1],root[dfs_las[u]], x,0,30));
        }
    }
    return 0;
}
