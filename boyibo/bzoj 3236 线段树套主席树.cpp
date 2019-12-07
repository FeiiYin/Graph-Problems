/// 每一个节点会有一棵主席树，但是静态的会爆内存，所以每一个节点建立，查询，再清空
/// 题意：查询 [l, r] 区间内值在 [a, b] 的数的个数以及不同数的个数
/// 一开始将所有的 query 根据 l, r 建立线段树，然后遍历线段树
/// 统计不同数字的个数，对于每一个位置记录它前一次出现的 pre ，查询 [l, r] 区间内有多少个 pre 小于 l 即可
/// 主席树是每一个 [1, i] 建立一颗线段树（链）
const int N=100005,M=1000005;
int n,m,l,r,a,b,num[N],last[N],pre[N],Hash[N],ans[M][2];
int cnt,root[N],sum[N*20],lc[N*20],rc[N*20];
struct Query{
    int a,b,l,id;
};
struct data{
    int a,v;
    bool operator < (const data &b) const{
        return a<b.a;
    }
}d[N];
vector<Query> q[N*4];
void addquery(int o,int l,int r,int L,int R,Query qry){
    if(L<=l&&R>=r){
        q[o].push_back(qry);
        return;
    }
    int mid=(l+r)/2;
    if(L<=mid){
        addquery(o*2,l,mid,L,R,qry);
    }
    if(R>mid){
        addquery(o*2+1,mid+1,r,L,R,qry);
    }
}
/// 主席树插入节点
void build(int y,int &x,int l,int r,int k){
    x=++cnt;
    sum[x]=sum[y]+1;
    lc[x]=lc[y];
    rc[x]=rc[y];
    if (l == r) { return; }
    int mid = (l + r) / 2;
    if (k<=mid) build(lc[y],lc[x],l,mid,k);
    else build(rc[y],rc[x],mid+1,r,k);
}
int query(int y,int x,int l,int r,int k){
    if(!x || l==r) { return 0; }
    int mid=(l+r)/2;
    if(k<=mid){
        return query(lc[y],lc[x],l,mid,k);
    }else{
        return sum[lc[x]]-sum[lc[y]]+query(rc[y],rc[x],mid+1,r,k);
    }
}
void solve(int o,int l,int r){
    if(q[o].size()){
        Hash[0]=0;
        for(int i=l;i<=r;i++){
            Hash[++Hash[0]]=num[i];
        }
        sort(Hash+1,Hash+Hash[0]+1);
        int s=0;
        for(int i=l;i<=r;i++){
            d[++s].a=lower_bound(Hash+1,Hash+Hash[0]+1,num[i])-Hash;
            d[s].v=pre[i];
        }
        sort(d+1,d+s+1);
        for(int i=1;i<=s;i++){
            /// 根据下标的 pre 去建立主席树
            build(root[i-1],root[i],0,n,d[i].v);
        }
        int a,b;
        for(int i=0;i<q[o].size();i++){
            /// 一定是 upper_bound - lower_bound 因为 a, b 有重复元素需要包括进去
            a = lower_bound(Hash+1, Hash+Hash[0]+1, q[o][i].a) - Hash;
            b = upper_bound(Hash+1, Hash+Hash[0]+1, q[o][i].b) - Hash - 1;
            /// 其实下面就等于 b - (a - 1) 因为每一个插进去的 val 都是 1 
            ans[q[o][i].id][0] += sum[root[b]] - sum[root[a-1]];
            /// 主席树 类似 差分的形式去求得答案，求得区间内 pre 小于 l 的个数
            ans[q[o][i].id][1] += query(root[a-1], root[b], 0, n, q[o][i].l);
        }
        memset(root,0,sizeof(int)*(Hash[0]+1));
        memset(sum,0,sizeof(int)*(cnt+1));
        memset(lc,0,sizeof(int)*(cnt+1));
        memset(rc,0,sizeof(int)*(cnt+1));
        cnt=0;
    }
    if(l==r){
        return;
    }
    int mid=(l+r)/2;
    solve(o*2,l,mid);
    solve(o*2+1,mid+1,r);
}
int main(){
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++){
        scanf("%d",&num[i]);
        pre[i]=last[num[i]];
        last[num[i]]=i;
    }
    for(int i=1;i<=m;i++){
        scanf("%d%d%d%d",&l,&r,&a,&b);
        addquery(1,1,n,l,r,(Query){a,b,l,i});
    }
    solve(1,1,n);
    for(int i=1;i<=m;i++){
        printf("%d %d\n",ans[i][0],ans[i][1]);
    }
    return 0;
}
