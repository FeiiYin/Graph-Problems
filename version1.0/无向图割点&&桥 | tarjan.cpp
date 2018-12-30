/*
 * @Samaritan_infi
 */
1.割点：1）当前节点为树根的时候，条件是“要有多余一棵子树”（如果这有一颗子树，去掉这个点也没有影响，如果有两颗子树，去掉这点，两颗子树就不连通了。）
2）当前节点U不是树根的时候，条件是“low[v]>=dfn[u]”，也就是在u之后遍历的点，能够向上翻，最多到u，如果能翻到u的上方，那就有环了，去掉u之后，
图仍然连通。
 保证v向上最多翻到u才可以
2.桥：若是一条无向边（u，v）是桥，
   1）当且仅当无向边（u，v）是树枝边的时候，需要满足dfn(u)<low(v)，也就是v向上翻不到u及其以上的点，那么u--v之间一定能够有1条或者多条边不能删去，
   因为他们之间有一部分无环，是桥。
 如果v能上翻到u那么u--v就是一个环，删除其中一条路径后，能然是连通的。
3.注意点：
1）求桥的时候：因为边是无方向的，所以父亲孩子节点的关系需要自己规定一下，
在tarjan的过程中if（v不是u的父节点） low[u]=min(low[u],dfn[v]);
因为如果v是u的父亲，那么这条无向边就被误认为是环了。
2）找桥的时候：注意看看有没有重边，有重边的边一定不是桥，也要避免误判。

#include<iostream>
using namespace std;
#include<cstdio>
#include<cstring>
#include<vector>
#define N 201
vector<int>G[N];
int n,m,low[N],dfn[N];
bool is_cut[N];
int father[N];
int tim=0;
void input()
{
    scanf("%d%d",&n,&m);
    int a,b;
    for(int i=1;i<=m;++i)
    {
        scanf("%d%d",&a,&b);
        G[a].push_back(b);/*邻接表储存无向边*/
        G[b].push_back(a);
    }
}
void Tarjan(int i,int Father)
{
    father[i]=Father;/*记录每一个点的父亲*/
    dfn[i]=low[i]=tim++;
    for(int j=0;j<G[i].size();++j)
    {
        int k=G[i][j];
        if(dfn[k]==-1)
        {
            Tarjan(k,i);
            low[i]=min(low[i],low[k]);
        }
        else if(Father!=k)/*假如k是i的父亲的话，那么这就是无向边中的重边，有重边那么一定不是桥*/
            low[i]=min(low[i],dfn[k]);//dfn[k]可能！=low[k]，所以不能用low[k]代替dfn[k],否则会上翻过头了。
    }
}
void count()
{
    int rootson=0;
    Tarjan(1,0);
    for(int i=2;i<=n;++i)
    {
        int v=father[i];
        if(v==1)
        rootson++;/*统计根节点子树的个数，根节点的子树个数>=2,就是割点*/
        else{
            if(low[i]>=dfn[v])/*割点的条件*/
            is_cut[v]=true;
        }
    }
    if(rootson>1)
    is_cut[1]=true;
    for(int i=1;i<=n;++i)
    if(is_cut[i])
    printf("%d\n",i);
    for(int i=1;i<=n;++i)
    {
        int v=father[i];
        if(v>0&&low[i]>dfn[v])/*桥的条件*/
        printf("%d,%d\n",v,i);
    }

}
int main()
{
    input();
    memset(dfn,-1,sizeof(dfn));
    memset(father,0,sizeof(father));
    memset(low,-1,sizeof(low));
    memset(is_cut,false,sizeof(is_cut));
    count();
    return 0;
}
