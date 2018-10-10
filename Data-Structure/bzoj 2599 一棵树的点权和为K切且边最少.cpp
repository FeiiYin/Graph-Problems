/*
 * @Samaritan_infi
 */
/// 题意: 给一棵树,每条边有权.求一条路径,权值和等于K,且边的数量最小.数据范围：N<=200000, K<=1000000
/// 题解: 开一个100W的数组t,t[i]表示权值为i的路径最少边数, 找到重心分成若干子树后， 得出一棵子树的所有点到根的权值和x，
///      到根a条边，用t[k-x]+a更新答案，全部查询完后, 然后再用所有a更新t[x],这样可以保证不出现点分治中的不合法情况
#include <bits/stdc++.h>
using namespace std;
#define MAXN 200100
#define INF 1000000000
struct node{int y,next,v;}e[MAXN*2];
int n,m,len,root,sum,ans,Link[MAXN],son[MAXN],vis[MAXN],t[MAXN*5],dis[MAXN],d[MAXN],f[MAXN];
char buf[1<<15],*fs,*ft;
inline char getc()  {return (fs==ft&&(ft=(fs=buf)+fread(buf,1,1<<15,stdin)),fs==ft)?0:*fs++;}
inline int read(){
    int x=0,f=1;  char ch=getc();
    while(!isdigit(ch))  {if(ch=='-')  f=-1;  ch=getc();}
    while(isdigit(ch))  {x=x*10+ch-'0';  ch=getc();}
    return x*f;
}
void insert(int x,int y,int v){e[++len].next=Link[x];  Link[x]=len;  e[len].y=y;  e[len].v=v;}
void getroot(int x,int fa){
    int ff=1;   son[x]=1;
    for(int i=Link[x];i;i=e[i].next)
        if(e[i].y!=fa&&!vis[e[i].y]){
            getroot(e[i].y,x);
            son[x]+=son[e[i].y];
            if(son[e[i].y]>sum/2)  ff=0;
        }
    if(sum-son[x]>sum/2)  ff=0;
    if(ff)  root=x;
}
void cal(int x,int fa){
    if(dis[x]<=m)  ans=min(ans,d[x]+t[m-dis[x]]);
    for(int i=Link[x];i;i=e[i].next)
        if(e[i].y!=fa&&!vis[e[i].y]){
            d[e[i].y]=d[x]+1;
            dis[e[i].y]=dis[x]+e[i].v;
            cal(e[i].y,x);
        }
}
void add(int x,int fa,int flag){
    if(dis[x]<=m){
        if(flag)  t[dis[x]]=min(t[dis[x]],d[x]);
        else t[dis[x]]=INF;
    }
    for(int i=Link[x];i;i=e[i].next)
        if(e[i].y!=fa&&!vis[e[i].y])
            add(e[i].y,x,flag);
}
void work(int x){
    vis[x]=1;  t[0]=0;
    for(int i=Link[x];i;i=e[i].next)
        if(!vis[e[i].y]){
            d[e[i].y]=1;  dis[e[i].y]=e[i].v;
            cal(e[i].y,0);
            add(e[i].y,0,1);
        }
    for(int i=Link[x];i;i=e[i].next)
        if(!vis[e[i].y])  add(e[i].y,0,0);    
    for(int i=Link[x];i;i=e[i].next)
        if(!vis[e[i].y]){
            sum=son[e[i].y];  root=0;
            getroot(e[i].y,0);
            work(e[i].y);
        }
}
int main(){
    n=read();  m=read();
    for(int i=1;i<=m;i++)t[i]=n;
    for(int i=1;i<n;i++){
        int x=read(),y=read(),v=read();
        x++;  y++;
        insert(x,y,v);  insert(y,x,v);
    }
    sum=ans=f[0]=n;
    getroot(1,0);
    work(root);
    if(ans==n)  printf("-1\n");
    else printf("%d\n",ans);
    return 0;
}
