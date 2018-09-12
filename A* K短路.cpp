#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

#define inf 0x7ffffff
#define MAXN 1010
#define MAXM 100005

struct Edge
{
    int u;//反向图的终止边
    int v;//终止点
    int c;//边权值
    int next;//下一个节点    
    int next1;//反向边下一个节点
    Edge(){}
    Edge(int u,int v,int c):u(u),v(v),c(c){}
}p[MAXM];
int head[MAXN];//链表头
int head1[MAXN];//反向
int e; //邻接表中边总数
int st,en,k;//起点，终点，第k短路
int n,m;
int dis[MAXN];//dis[v]表示的是终点到v的距离，即估价函数g
bool vis[MAXN];

struct pro
{
    int v,c;//v是终点,c是起点到v的距离
    pro(){}
    pro(int v,int c):v(v),c(c){}
    bool operator < (const pro& a) const 
    {
        return c+dis[v] > a.c+dis[a.v];//最小值优先队列
    }
};

void clear()//初始化
{
    memset(head,-1,sizeof(head));
    memset(head1,-1,sizeof(head1));
    e=0;
}
void addEdge(int u,int v,int c)//加边
{
    p[e]=Edge(u,v,c);
    p[e].next1=head1[v];head1[v]=e;
    p[e].next=head[u];head[u]=e++;
}

priority_queue<pro> que;
void dij(int src)//求估价函数
{
    memset(vis,0,sizeof(vis));
    for(int i=1;i<=n;i++)
        dis[i]=inf;
    dis[src]=0;
    while(!que.empty())
        que.pop();
    que.push(pro(src,0));
    while(!que.empty())
    {
        pro cur = que.top();
        que.pop();
        if(vis[cur.v])
            continue;
        vis[cur.v]=1;
        for(int i=head1[cur.v];i+1;i=p[i].next1)
        {
            if(dis[p[i].u] > dis[cur.v] + p[i].c)
            {
                dis[p[i].u] = dis[cur.v] + p[i].c;
                que.push(pro(p[i].u,0));
            }
        }
    }
}

int a_star(int src)
{
    while(!que.empty())
        que.pop();
    que.push(pro(src,0));
    while(!que.empty())
    {
        pro cur = que.top();
        que.pop();
        if(cur.v==en)
        {
            if(k>1)//相当于求k次最短路
                k--;
            else
                return cur.c;
        }

        for(int i=head[cur.v];i+1;i=p[i].next)//将所有与u相连接的点都加入队列
            que.push(pro(p[i].v,cur.c+p[i].c));
    }
    return -1;
}

int main()
{
    int u,v,c;
    while(scanf("%d%d",&n,&m) != EOF)
    {
        clear();
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&c);
            addEdge(u,v,c);
        }
        scanf("%d%d%d",&st,&en,&k);
        dij(en);
        if(dis[st]==inf)
        {
            puts("-1");
            continue;
        }
        if(st == en)
            k++;
        printf("%d\n",a_star(st));
    }
    return 0;
}
