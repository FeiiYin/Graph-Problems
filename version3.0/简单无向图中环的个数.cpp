#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 20;
int maze[maxn][maxn];
// dp[s][i] : 表示ｓ状态下以　s 状态的最小顶点和顶点i构成的简单环　(这样仍然会重复计算2次)
ll dp[1<<maxn][maxn];
int n,m;
int main()
{
    while(~scanf("%d%d",&n,&m))
    {
        memset(dp,0,sizeof(dp));
        memset(maze,0,sizeof(maze));
        for(int i=0,u,v;i<m;i++) {
            scanf("%d%d",&u,&v);
            u--,v--;
            maze[u][v] = maze[v][u] = 1;
        }
        ll ans = 0;
        for(int i=0;i<n;i++) dp[1<<i][i] = 1;
        for(int s=1;s<(1<<n);s++) {
            int pre = log2(s & -s);
            for(int i=pre;i<n;i++) if(dp[s][i]){ /// 枚举结尾的顶点
                for(int j = pre;j<n;j++) if(maze[i][j]){ /// 枚举接下来要连接的顶点
                    if(s & (1<<j)) { ///　节点j在当前状态中
                        if (((1<<i)|(1<<j))==s) continue; /// 排除两个节点成环的情况
                        if(j == pre) ans += dp[s][i];
                    }                    
                    else { /// 节点j不在当前状态中
                        dp[s|(1<<j)][j] += dp[s][i];
                    }
                }
            }
        }
        printf("%lld\n",ans/2);
    }
    return 0;
}
