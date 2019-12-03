/// 树上每一条边有n个颜色，问两边之间颜色不同可以加一，求两点之间路径的最大值
#include<map>
#include<cstdio>
#include<cctype>
#include<vector>
inline int getint() {
    register char ch;
    while(!isdigit(ch=getchar()));
    register int x=ch^'0';
    while(isdigit(ch=getchar())) x=(((x<<2)+x)<<1)+(ch^'0');
    return x;
}
const int N=5e5+1,logN=19;
int tot,anc[N][logN],dep[N],f[N][2],g[N][2];
int seg[N][logN][2][2],w[N][logN][2];
std::vector<int> e[N];
inline void add_edge(const int &u,const int &v) {
    e[u].push_back(v);
    e[v].push_back(u);
}
std::map<std::pair<int,int>,std::pair<int,int>> map;
inline int lg2(const float &x) {
    return ((unsigned&)x>>23&255)-127;
}
inline void up(int &x,const int &y) {
    x=std::max(x,y);
}
void dfs(const int &x,const int &par) {
    if(par) {
        int u=x,v=par;
        if(u>v) std::swap(u,v);
        const auto &p=map[std::make_pair(u,v)];
        if(p.first) {
            w[x][0][0]=p.first;
            seg[x][0][0][0]=1;
        }
        if(p.second) {
            w[x][0][1]=p.second;
            seg[x][0][1][1]=1;
        }
    }
    anc[x][0]=par;
    dep[x]=dep[par]+1;
    for(register int i=1;i<=lg2(dep[x]);i++) {
        anc[x][i]=anc[anc[x][i-1]][i-1];
        w[x][i][0]=w[anc[x][i-1]][i-1][0];
        w[x][i][1]=w[anc[x][i-1]][i-1][1];
        for(register int j=0;j<2;j++) {
            if(!w[x][0][j]) continue;
            for(register int k=0;k<2;k++) {
                if(!w[x][i][k]) continue;
                for(register int l=0;l<2;l++) {
                    if(!w[x][i-1][l]) continue;
                    for(register int m=0;m<2;m++) {
                        if(!w[anc[x][i-1]][0][m]) continue;
                        up(seg[x][i][j][k],seg[x][i-1][j][l]+seg[anc[x][i-1]][i-1][m][k]-(w[x][i-1][l]==w[anc[x][i-1]][0][m]));
                    }
                }
            }
        }
    }
    for(auto &y:e[x]) {
        if(y!=par) dfs(y,x);
    }
}
inline int query(int u,int v) {
    if(u==v) return 0;
    if(dep[u]<dep[v]) std::swap(u,v);
    f[u][0]=f[u][1]=g[u][0]=g[u][1]=0;
    f[v][0]=f[v][1]=g[v][0]=g[v][1]=0;
    for(register int i=lg2(dep[u]-dep[v]);i>=0;i--) {
        const int &p=anc[u][i];
        if(dep[p]<dep[v]) continue;
        f[p][0]=f[p][1]=g[p][0]=g[p][1]=0;
        for(register int j=0;j<2;j++) {
            for(register int k=0;k<2;k++) {
                if(!w[u][0][k]) continue;
                for(register int l=0;l<2;l++) {
                    if(f[u][j]+seg[u][i][k][l]-(g[u][j]==w[u][0][k])>f[p][l]) {
                        f[p][l]=f[u][j]+seg[u][i][k][l]-(g[u][j]==w[u][0][k]);
                        g[p][l]=w[u][i][l];
                    }
                }
            }
        }
        u=p;
    }
    if(u==v) return std::max(f[u][0],f[u][1])-1;
    for(register int i=lg2(dep[u]);i>=0;i--) {
        const int &p=anc[u][i],&q=anc[v][i];
        if(p==q) continue;
        f[p][0]=f[p][1]=g[p][0]=g[p][1]=0;
        for(register int j=0;j<2;j++) {
            for(register int k=0;k<2;k++) {
                if(!w[u][0][k]) continue;
                for(register int l=0;l<2;l++) {
                    if(f[u][j]+seg[u][i][k][l]-(g[u][j]==w[u][0][k])>f[p][l]) {
                        f[p][l]=f[u][j]+seg[u][i][k][l]-(g[u][j]==w[u][0][k]);
                        g[p][l]=w[u][i][l];
                    }
                }
            }
        }
        f[q][0]=f[q][1]=g[q][0]=g[q][1]=0;
        for(register int j=0;j<2;j++) {
            for(register int k=0;k<2;k++) {
                if(!w[v][0][k]) continue;
                for(register int l=0;l<2;l++) {
                    if(f[v][j]+seg[v][i][k][l]-(g[v][j]==w[v][0][k])>f[q][l]) {
                        f[q][l]=f[v][j]+seg[v][i][k][l]-(g[v][j]==w[v][0][k]);
                        g[q][l]=w[v][i][l];
                    }
                }
            }
        }
        u=p;
        v=q;
    }
    int p=anc[u][0];
    f[p][0]=f[p][1]=g[p][0]=g[p][1]=0;
    for(register int j=0;j<2;j++) {
        for(register int k=0;k<2;k++) {
            if(!w[u][0][k]) continue;
            if(f[u][j]+seg[u][0][k][k]-(g[u][j]==w[u][0][k])>f[p][k]) {
                f[p][k]=f[u][j]+seg[u][0][k][k]-(g[u][j]==w[u][0][k]);
                g[p][k]=w[u][0][k];
            }
        }
    }
    p=0;
    f[p][0]=f[p][1]=g[p][0]=g[p][1]=0;
    for(register int j=0;j<2;j++) {
        for(register int k=0;k<2;k++) {
            if(!w[v][0][k]) continue;
            if(f[v][j]+seg[v][0][k][k]-(g[v][j]==w[v][0][k])>f[p][k]) {
                f[p][k]=f[v][j]+seg[v][0][k][k]-(g[v][j]==w[v][0][k]);
                g[p][k]=w[v][0][k];
            }
        }
    }
    int ans=0;
    for(register int i=0;i<2;i++) {
        for(register int j=0;j<2;j++) {
            up(ans,f[anc[u][0]][i]+f[0][j]-(g[anc[u][0]][i]==g[0][j]));
        }
    }
    return ans-1;
}
int main() {
    const int n=getint(),m=getint();
    for(register int i=0;i<m;i++) {
        int u=getint(),v=getint(),c=getint();
        if(u>v) std::swap(u,v);
        auto &p=map[std::make_pair(u,v)];
        if(p.first<0) continue;
        if(p.first==c||p.second==c) continue;
        if(p.first==0) {
            add_edge(u,v);
            p.first=c;
            continue;
        }
        if(p.second==0) {
            p.second=c;
            continue;
        }
        p.first=--tot;
        p.second=0;
    }
    dfs(1,0);
    const int q=getint();
    for(register int i=0;i<q;i++) {
        printf("%d\n",query(getint(),getint()));
    }
    return 0;
}
