/*
 * @Samaritan_infi
 */
/// 题意：给出一个数组，查询某一段的GCD是否猜测正确，假如这段最多有一个数不是所有数GCD的倍数，那么就算猜对了，否则就是猜错。 
/// 思路：用线段树维护每一段的GCD，单点更新，向下查询的时候，如果左子树的GCD不能整除x,就说明至少在左子树这个区间有一个数不是x的倍数，
/// 这些数可能在要找的区间[a,b]里，也可能不在，而是在左子树-[a,b]的集合里，如果是后者，线段树在寻找GCD的过程中就会将这些外面的部分自动撇弃，
/// 如果是前者，线段树找到最后l==r的时候，就说明这个数就是那个毒瘤，现在ans++,如果ans>1，那么不用再查询，直接输出“NO”
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e6 + 5;
#define ls rt<<1
#define rs rt<<1|1
using namespace std;
int v[N << 2], a[N], ans;
void push_up(int rt) { v[rt] = __gcd(v[ls], v[rs]); }
void bulid(int l, int r, int rt) {
    if(l == r) { v[rt] = a[l]; return; }
    int mid = (l + r) >> 1;
    bulid(l, mid, ls);
    bulid(mid+1, r, rs);
    push_up(rt);
}
void modfiy(int l, int r, int rt, int pos, int val) {
    if(l == r) { v[rt] = val; return; } int mid = (l+r)>>1;
    if(pos <= mid) modfiy(l,mid,ls,pos,val);
    else modfiy(mid+1,r,rs,pos,val);
    push_up(rt);
}
void query(int l,int r,int s,int t,int rt,int x) {
    if(l==r) {ans++;return ;}
    if(ans>1) return;
    int mid = (l+r)>>1;
    if(s<=mid && v[ls]%x!=0){query(l,mid,s,t,ls,x);}
    if(t>mid && v[rs]%x!=0) {query(mid+1,r,s,t,rs,x);}
}
int main() {
    int n, m, op, l, r, x;
    scanf("%d",&n);
    for(int i = 1; i <= n; i++) scanf("%d", &a[i]);
    memset(v, 0, sizeof(0));
    bulid(1, n, 1);
    scanf("%d", &m);
    for(int i=0; i<m; i++){
        scanf("%d",&op);
        if(op==1) {
            ans=0;
            scanf("%d%d%d",&l,&r,&x);
            query(1, n, l, r, 1, x);
            if(ans<=1) cout<<"YES"<<endl;else cout<<"NO"<<endl;
        } else {
            int pos;
            scanf("%d %d", &pos, &x);
            modfiy(1, n, 1, pos, x);
        }
    }
    return 0;
}
