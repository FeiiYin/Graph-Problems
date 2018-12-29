介绍边双连通分量的求解算法： 
边双连通分量的求解非常简单，因为边双连通分量之间没有公共边，而且桥不在任意一个边双连通分量中，所以算法十分简单，
即先一次DFS找到所有桥，再一次DFS（排除了桥）找到边双连通分量。 
PS：当然可以用一次DFS实现。 
struct Edge{
    int u,v;
    Edge(int u=0,int v=0):u(u),v(v){}
}e[maxm];
int n,m,stamp,dfn[maxn],low[maxn],bccno[maxn],bcc_cnt;
vector<int> vec[maxn],bcc[maxn];
bool g[maxn][maxn],isbridge[maxm];

void tarjan(int index,int fa)
{
    int tmp;
    dfn[index]=low[index]=++stamp;
    for(int i=0;i<vec[index].size();i++)
    {
        tmp=e[vec[index][i]].v;
        if(!dfn[tmp])
        {
            tarjan(tmp,index);
            low[index]=min(low[index],low[tmp]);
            if(low[tmp]>dfn[index])
                isbridge[vec[index][i]]=isbridge[vec[index][i]^1]=1;
        }
        else if(dfn[tmp]<dfn[index] && tmp!=fa)
        {
            low[index]=min(low[index], dfn[tmp]);
        }
    }
}

void dfs(int index)
{
    dfn[index]=1;
    bccno[index]=bcc_cnt;
    for(int i=0;i<vec[index].size();i++)
    {
        int tmp=vec[index][i];
        if(isbridge[tmp])
            continue;
        if(!dfn[e[tmp].v])
        {
            dfs(e[tmp].v);
        }
    }
}

void find_ebcc(){
    bcc_cnt=stamp=0;
    memset(dfn,0,sizeof(dfn));
    memset(low,0,sizeof(low));
    memset(isbridge,0,sizeof(isbridge));
    memset(bccno,0,sizeof(bccno));
    memset(bcc,0,sizeof(bcc));
    for(int i=1;i<=n;i++)
        if(!dfn[i])
            tarjan(i, -1);
    memset(dfn,0,sizeof(dfn));
    for(int i=1;i<=n;i++)
    {
        if(!dfn[i])
        {
            bcc_cnt++;
            dfs(i);
        }
    }               
}
