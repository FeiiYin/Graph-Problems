题意：有一个水罐车，长为n宽为1，无限高，现在在里面插入n-1个板子，把它分成n个底为1×1的块，有m个条件，x，y，z，表示如果第x个块里高为y+0.5的地方有水，
那么z为1，没水，z为0。问这m个条件最多有多少个是同时正确的。如果水高于板子会溢出。 
做法：左偏树是什么，啊，真香，左偏树可以合并两个有序集合并保证有序，对于某一个区间q，它可以看作是有这个区间中最高的点分开的两个区间q1，q2，这个区间
的最大贡献：1，水不超过这个最高的点，就是这q1，q2水不溢出的情况下的最大贡献，2：水超过了这个点，枚举高这超过这个最高点但是不大于q区间的两边的板子，
的最大值，合并区间的可选择点可以用左偏树。
#include<bits/stdc++.h>
using namespace std;
const int N = 2e5+100;
struct node{
    int h,op;
    bool operator<(const node &p)const{
        if(h != p.h) return h < p.h;
        return op < p.op;
    }
};
node pt[N];
int l[N],r[N],head[N],pre[N],v[N],mx[N],h[N],w[N];
int ls[N],rs[N];
int que[N];
int Find(int x){return x == pre[x]?x:pre[x] = Find(pre[x]); }
int merge(int x,int y){
    if(x == 0) return y;
    if(y == 0) return x;
    if(pt[y] < pt[x]) swap(x,y);
    r[x] = merge(r[x],y);
    if(h[l[x]] < h[r[x]]) swap(l[x],r[x]);
    h[x] = h[l[x]]+1;
    return x;
}
int del(int x){
    return merge(l[x],r[x]);
}
int main(){
    int T;
    cin >> T;
    for(int kase = 1;kase <= T;kase ++){
        int n,m;
        scanf("%d %d",&n,&m);
        for(int i = 1;i <n;i ++){
            scanf("%d",&w[i]);
        }
        w[0] = 1e9+7;
        w[n] = 1e9+7;
        vector<pair<int,int> > vp;
        for(int i = 1;i < n;i ++) vp.push_back({w[i],i});
        sort(vp.begin(),vp.end());
        memset(head,0,sizeof(head));
        memset(h,0,sizeof(h));
        memset(v,0,sizeof(v));
        memset(mx,0,sizeof(mx));
        memset(l,0,sizeof(l));
        memset(r,0,sizeof(r));
        for(int i = 1;i <= n;i ++) pre[i] = rs[i] = i,ls[i] = i-1;
        for(int i = 1;i <= m;i ++){
            int x;
            scanf("%d %d %d",&x,&pt[i].h,&pt[i].op);
            if(pt[i].op == 0) v[x]++;
            head[x] = merge(head[x],i);
        }
        for(int i = 1;i <= n;i ++){
            int mn = min(w[i],w[i-1]);
            int now = v[i];
            int res = now;
            while(head[i]&&pt[head[i]].h < mn){
                if(pt[head[i]].op == 0) now --;
                else now ++;
                res = max(res,now);
                head[i] = del(head[i]);
            }
            mx[i] = res;
            v[i] = now;
        }
        for(int i = 0;i < n-1;i ++){
            int pos = vp[i].second;
            int fa = Find(pos),fb = Find(pos+1);
            int L = ls[fa],R = rs[fb];
            int mn = min(w[L],w[R]);
            pre[fb] = fa;
            ls[fa] = L,rs[fa] = R;
            head[fa] = merge(head[fa],head[fb]);
            int now = v[fa]+v[fb];
            int res = mx[fa]+mx[fb];
            while(head[fa]&&pt[head[fa]].h < mn){
                if(pt[head[fa]].op == 0) now --;
                else now ++;
                res = max(now,res);
                head[fa] = del(head[fa]);
            }
            v[fa] = now;
            mx[fa] = res;
        }
        printf("Case #%d: ",kase);
        cout << mx[1] << endl;
    }
    return 0;
}
