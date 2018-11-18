///题意：硬币翻转，只能翻连续k个朝面一样的.要么同时朝上，要么同时朝下，问原串能不能翻转到新串
#include <bits/stdc++.h>
using namespace std;
const int maxn=1e6+5;
char s[maxn],t[maxn],sum[maxn];
int n,k;
inline int low_bit (int x) { return x & -x;}
void up_date(int now) {
    for (; now <= n; now += low_bit(now)) sum[now] ^= 1;
}
int ask(int now) {
    int res=0;
    for (; now; now -= low_bit(now)) res ^= sum[now];
    return res;
}
int main() {
    while(scanf("%d%d",&n,&k)!=EOF) {
        scanf("%s",s+1); scanf("%s",t+1);
        memset(sum,0,sizeof(sum));
        if(k==1) { printf("Yes\n"); continue; }
        int f=0;
        for(int i=1;i<=n-k+1;i++) {
            int tp = (ask(i-1) ^ ask(max(0, i-k)));
            if((s[i] != t[i] && tp == 0) || (s[i] == t[i] && tp == 1)) up_date(i);
        }
        for(int i=n-k+2;i<=n;i++) {
            int tp = (ask(i-1) ^ ask(max(0,i-k)));
            if((s[i] != t[i] && tp==0) || (s[i] == t[i] && tp==1)) {
                f=1;  break;
            }
        }
        if(!f) printf("Yes\n");
        else printf("No\n");
    }
    return 0;
}
/*
6 2
000000
101101
8 3
10101010
01010101
*/
