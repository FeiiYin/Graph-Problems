解法一：
假设答案为k ，则要求解的问题是：选出一个合适的点集V和边集E，令(|E| - k * |V|) 取得最大值。
所谓“合适”是指满足如下限制：若选择某条边，则必选择其两端点。
建图:以原图的边作为左侧顶点，权值为1；原图的点作为右侧顶点，权值为-k(相当于 支出k)。
若原图中存在边(u,v)，则新图中添加两条边([uv]-->u), ([uv]-->v)，转换为最大权闭合子图。
解法二：
把原图中的无向边转换成两条有向边，容量为1。
设一源点，连接所有点，容量为U（取m）。
设一汇点，所有点连接汇点，容量为 U+2g-dv 。
二分枚举最大密度g，其中dv为v的度。
判断(U*n-MaxFlow)/2.>=0。
最后跳出的L就是最大密度。
拿这个L再重新建图，求最大流。
然后从s出发bfs或者dfs，走残留容量大于0的边，所有能到达的点就是答案。

	#include<cstdio>
	#include<cstring>
	#define N 1005
	#define inf 1<<30
	#define min(a,b) ((a)<(b)?(a):(b))
	int n,m,s,t,num,adj[N],dis[N],q[N],d[N];
	struct edge
	{
		int v,pre;
		double w;
	}e[10005];
	struct node
	{
		int u,v;
	}E[N];
	void insert(int u,int v,double w)
	{
		e[num]=(edge){v,adj[u],w};
		adj[u]=num++;
		e[num]=(edge){u,adj[v],0};
		adj[v]=num++;
	}
	int bfs()
	{
		int i,x,v,head=0,tail=0;
   	memset(dis,0,sizeof(dis));
		dis[s]=1;
		q[++tail]=s;
		while(head!=tail)
		{
			x=q[head=(head+1)%N];
			for(i=adj[x];~i;i=e[i].pre)
				if(e[i].w>0&&!dis[v=e[i].v])
				{
					dis[v]=dis[x]+1;
					if(v==t)
						return 1;
					q[tail=(tail+1)%N]=v;
				}
		}
		return 0;
	}
	double dfs(int x,double limit)
	{
		if(x==t)
			return limit;
		int i,v;
		double tmp,cost=0;
		for(i=adj[x];~i&&cost<limit;i=e[i].pre)
			if(e[i].w>0&&dis[x]==dis[v=e[i].v]-1)			{
				tmp=dfs(v,min(limit-cost,e[i].w));
				if(tmp)				{
					e[i].w-=tmp;
					e[i^1].w+=tmp;
					cost+=tmp;
				}
				else
					dis[v]=-1;
			}
		return cost;
	}
	double Dinic()
	{
		double ans=0;
		while(bfs())
			ans+=dfs(s,inf);
		return ans;
	}
	int ok(double mid)
	{
		int i;
		memset(adj,-1,sizeof(adj));
		num=0;
		s=0;
		t=n+1;
		for(i=1;i<=m;i++)
		{
			insert(E[i].v,E[i].u,1);
			insert(E[i].u,E[i].v,1);
		}
		for(i=1;i<=n;i++)
		{
			insert(s,i,m);
			insert(i,t,m+2*mid-d[i]);
		}
		return (m*n-Dinic())/2.>=1e-6;
	}
	bool f[N];
	void dfs(int x)
	{
  	f[x]=1;
		for(int v,i=adj[x];~i;i=e[i].pre)
			if(!f[v=e[i].v]&&e[i].w>0)
				dfs(v);
	}
	int main()
	{
		while(~scanf("%d%d",&n,&m))
		{
			int i,ans=0;
			memset(d,0,sizeof(d));
			for(i=1;i<=m;i++)
			{
				scanf("%d%d",&E[i].u,&E[i].v);
				d[E[i].u]++;
				d[E[i].v]++;
			}
			if(!m)
			{
				puts("1\n1\n");
				continue;
			}
			double l=0,r=m,mid;
			while(r-l>=1./n/n)
			{
				mid=(l+r)/2.;
				if(ok(mid))
					l=mid;
				else
  				r=mid;
			}
			ok(l);
			memset(f,0,sizeof(f));
			dfs(s);
			for(i=1;i<=n;i++)
				ans+=f[i];
			printf("%d\n",ans);
			for(i=1;i<=n;i++)
				if(f[i])
					printf("%d\n",i);
  	}
	}
