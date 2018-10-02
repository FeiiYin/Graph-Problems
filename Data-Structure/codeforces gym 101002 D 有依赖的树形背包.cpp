/*
 * @Samaritan_infi
 */
/// 题目链接: http://codeforces.com/gym/101002  D
/// 题意: 给 n 个人, 有 薪资 salary 产量 produce 推荐人 recommend
///       选中恰好 k + 1 个人, 使选的人的推荐人一定也在其中, 使 sum produce / sum salary 最大
///       且题目保证推荐人的 id 比其小
/// 题解: 对于结果进行分数规划, 二分答案, check 的时候进行依赖树形dp
///       由于这里是可以为负的, 所以一定要注意 max 0 的操作
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N = 3e3 + 5;
const double eps = 1e-5;
vector<int> edge[N];
int sal[N], pro[N], rec[N];

double dp[N][N], m;
int k, n;
int siz[N], dfn[N], tot, rev[N];
void init_dfs (int x) {
    siz[x] = 1;
    dfn[x] = ++tot;
    rev[tot] = x;
    for (int to : edge[x]) {
        init_dfs(to);
        siz[x] += siz[to];
    }
}
bool check (double m) {
    for (int i = 0; i <= k; ++i) dp[n + 2][i] = -10000000;
    dp[n + 2][0] = 0;  dp[1][k] = -10000000;
    for (int i = tot; i >= 1; --i) {
        int x = rev[i];
        for (int p = 0; p <= k; p++) {
            if (p < 1) dp[i][p] = max(dp[i + siz[x]][p], 0.0); /// 注意这里要 max 0
            else dp[i][p] = max(dp[i+siz[x]][p], dp[i+1][p-1] + pro[x] - m * sal[x]);
        }
    }
    return dp[1][k] > 0;
}
int main () {
    scanf("%d %d", &k, &n); k++;
    for (int i = 1; i <= n; ++i) {
        scanf("%d %d %d", &sal[i], &pro[i], &rec[i]);
        edge[rec[i]].push_back(i);
    }
    tot = 0;
    init_dfs(0);
    double l = 0, r = k * 10000.0, mid;
    while (r - l > eps) {
        mid = (l + r) / 2;
        if (check(mid)) l = mid;
        else r = mid;
    }
    if (check(r)) l = r;
    double ans = (long long)(l * 1000 + 0.5); ans = ans / 1000;
    printf("%.3f\n", ans);
    return 0;
}

/// 有依赖的树形背包dp
/// 题意: 有一个树，每一个节点代表一个物品，每个物品有重量和价值，每个物品必须先选父亲才能选自己。求给定重量内最大价值。
/// 题解: 先求树的dfs序，对于dfs序上每一个点，考虑如果选自己那么自己子树内就可以选，否则只有在这棵子树外面才可以选。
///       记f[i][j]为dfs序中第i个点及以后的dfs序,大小为j的联通块的最大权值，得出f[i][j]=max(f[i+1][j-w[i]]+v[st[i]],f[i+N[st[i]]][j])

///w=weight v=value
#define N 5004
int n,m,fa[N],w[N],v[N],fc[N],nc[N],md[N],siz[N],dp[N][N];
int st[N],t=0,dl[N]; /// dfs 序, 反转值 == rev
void ass(int x) {int f=fa[x]; if(f) nc[x]=fc[f], fc[f]=x;} /// 插入边, 改下
void dfs(int p) {
    siz[p]=1; st[p]=++t; dl[t]=p;
    for(int c=fc[p];c;c=nc[c]) {
        dfs(c); siz[p]+=siz[c];
    }
}
int main() {
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++) scanf("%d%d%d",fa[i],w[i],v[i]), ass(i);
    dfs(1);
    for(int p=0;p<=m;p++) dp[n+1][p]=dp[n+2][p]=-1000000000;
    dp[n+1][0]=0; 
    for(int i = n; i>=1; i--) {
        int x=dl[i];
        for(int p=0;p<=m;p++) {
            if(p < w[x]) dp[i][p]=max(dp[i+siz[x]][p],0);
            else dp[i][p]=max(max(dp[i+siz[x]][p],dp[i+1][p-w[x]]+v[x]),0); /// 注意这里要 max 0
        }
    }
    printf("%d\n",dp[1][m]);
}
