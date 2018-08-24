/*
用到了
heavy−light decomposition
heavy−light decomposition
树链剖分
把轻边子树的信息合并到重链上的点里
因为每次都是先dfs轻儿子再dfs重儿子，只有重儿子子树的贡献保留，所以可以保证dfs到每颗子树时当前全局维护的信息不会有别的子树里的
先递归计算轻儿子子树，递归结束时消除他们的贡献
再递归计算重儿子子树，保留他的贡献
再计算当前子树中所有轻子树的贡献
更新答案
如果当前子树是父节点的轻子树，消除当前子树的贡献
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
#define pii pair<int, ll>
#define MP make_pair 
#define fir first
#define sec second
const int N=1e5+5;
int read(){
    char c=getchar();int x=0,f=1;
    while(c<'0'||c>'9'){if(c=='-')f=-1; c=getchar();}
    while(c>='0'&&c<='9'){x=x*10+c-'0'; c=getchar();}
    return x*f;
}

int n, a[N];
struct edge{int v, ne;}e[N<<1];
int cnt, h[N];
inline void ins(int u, int v) {
    e[++cnt]=(edge){v, h[u]}; h[u]=cnt;
    e[++cnt]=(edge){u, h[v]}; h[v]=cnt;
}
int size[N], mx[N], big[N];
void dfs(int u, int fa) {
    size[u]=1;
    for(int i=h[u];i;i=e[i].ne) 
        if(e[i].v != fa) {
            dfs(e[i].v, u);
            size[u] += size[e[i].v];
            if(size[e[i].v] > size[mx[u]]) mx[u] = e[i].v;
        }
}

int cou[N], Max; ll ans[N];
pii f[N];
void update(int u, int fa, int val) {
    int &c = cou[a[u]];
    f[c].fir --; f[c].sec -= a[u];
    c += val;
    f[c].fir ++; f[c].sec += a[u];
    if(val==1) Max = max(Max, c);
    else if(!f[Max].fir) Max--;

    for(int i=h[u];i;i=e[i].ne) 
        if(e[i].v != fa && !big[e[i].v]) update(e[i].v, u, val);
}

void dfs(int u, int fa, int keep) {
    for(int i=h[u];i;i=e[i].ne) 
        if(e[i].v != fa && e[i].v != mx[u]) dfs(e[i].v, u, 0);
    if(mx[u]) dfs(mx[u], u, 1), big[mx[u]] = 1;
    update(u, fa, 1);
    ans[u] = f[Max].sec;
    big[mx[u]] = 0;
    if(!keep) update(u, fa, -1);
}
int main() {
    //freopen("in","r",stdin);
    n=read();
    for(int i=1; i<=n; i++) a[i]=read();
    for(int i=1; i<n; i++) ins(read(), read());
    dfs(1, 0);
    dfs(1, 0, 1);
    for(int i=1; i<=n; i++) printf("%I64d ",ans[i]);
}
