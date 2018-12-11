/// 解析图见模板
#include <bits/stdc++.h>
using namespace std;
typedef long long           LL ;
typedef unsigned long long ULL ;
const int    maxn = 1e5 + 10   ;
const int    inf  = 0x3f3f3f3f ;
const int    npos = -1         ;
const int    mod  = 1e9 + 7    ;
const int    mxx  = 100 + 5    ;
const double eps  = 1e-6       ;
const double PI   = acos(-1.0) ;

ULL T, n, prime[maxn], nd, theta2;
bool vis[maxn];
int tot, cnt;
void init(int top){
    tot=0;
    memset(vis,true,sizeof(vis));
    for(int i=2;i<=top;i++){
        if(vis[i]){
            prime[tot++]=(ULL)i;
        }
        for(int j=0;j<tot&&(i*prime[j]<=top);j++){
            vis[i*prime[j]]=false;
            if(i%prime[j]==0)
                break;
        }
    }
}
int main(){
    // freopen("in.txt","r",stdin);
    // freopen("out.txt","w",stdout);
    init(1e5+1);
    while(~scanf("%llu",&T)){
        while(T--){
            scanf("%llu",&n);
            theta2=1ULL;
            nd=n;
            for(int i=0;i<tot && prime[i]*prime[i]<=n;i++)
                if(n%prime[i]==0){
                    ULL p=prime[i], alpha=0ULL;
                    ULL sigma=1ULL, square=1ULL;
                    while(n%p==0ULL){
                        alpha++;
                        square*=p;
                        sigma+=square*square;
                        n/=p;
                    }
                    nd*=alpha+1ULL;
                    theta2*=sigma;
                }
            if(n>1){
                nd*=2ULL;
                theta2*=(1ULL+n*n);
            }
            printf("%llu\n",theta2-nd);
        }
    }
    return 0;
}
