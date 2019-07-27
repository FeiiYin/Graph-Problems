题意：给定若干个集合，判断l到r的区间内是否都能异或表示出x
即l到r区间的线性基交能不能表示，下面用离线后前后不断合并，也可直接建立线段树在线维护

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned int uint;
#define N 50000 + 5
#define LOGN 17
 
int n, m, Fa[N];
bool Ans[N];
 
struct Query
{
    int l, x, id;
    Query() {}
    Query(int l, int x, int id) : l(l), x(x), id(id) {}
};
 
vector<Query> Vec[N];
 
struct LinearBase
{
    int sz;
    uint W[33], C[33];
    LinearBase() : sz(0) {}
    uint Residual(uint x, uint *c = NULL) const
    {
        for (int i = 0; i < sz; i ++)
            if ((x ^ W[i]) < x)
            {
                x ^= W[i];
                if (c != NULL)
                    *c ^= C[i];
            }
        return x;
    }
    uint Get(uint state) const
    {
        uint res = 0;
        for (int i = 0; i < sz; i ++)
            if ((state >> i) & 1)
                res ^= W[i];
        return res;
    }
    bool operator == (const LinearBase &obj) const
    {
        if (sz != obj.sz)
            return false;
        for (int i = 0; i < sz; i ++)
            if (W[i] != obj.W[i])
                return false;
        return true;
    }
    void Add(uint x, uint c = 0)
    {
        x = Residual(x, &c);
        if (!x) return ;
        W[sz] = x, C[sz] = c, sz ++;
        for (int i = sz - 1; i; i --)
        {
            if (W[i] > W[i - 1])
                swap(W[i], W[i - 1]), swap(C[i], C[i - 1]);
            else
            {
                for (int j = i - 1; j >= 0; j --)
                    if ((W[j] ^ W[i]) < W[j])
                        W[j] ^= W[i], C[j] ^= C[i];
                break ;
            }
        }
    }
    static LinearBase Merge(const LinearBase &lhs, const LinearBase &rhs)
    {
        if (lhs.sz == 32)
            return rhs;
        else if (rhs.sz == 32)
            return lhs;
        else if (lhs == rhs)
            return lhs;
        LinearBase tmp, res;
        for (int i = 0; i < lhs.sz; i ++)
        {
            uint t = rhs.Residual(lhs.W[i]), c = 1u << i;
            uint r = tmp.Residual(t, &c);
            if (r == 0)
                res.Add(lhs.Get(c));
            else tmp.Add(r, c);
        }
        return res;
    }
}B[N];
 
int Find(int x)
{
    if (x == Fa[x])
        return x;
    int res = Find(Fa[x]);
    B[x] = LinearBase::Merge(B[x], B[Fa[x]]);
    return Fa[x] = res;
}
 
int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 1, sz; i <= n; Fa[i] = i, i ++)
        for (scanf("%d", &sz); sz; sz --)
        {
            uint x;
            scanf("%u", &x);
            B[i].Add(x);
        }
    for (int i = 1, l, r, x; i <= m; i ++)
    {
        scanf("%d%d%d", &l, &r, &x);
        Vec[r].emplace_back(l, x, i);
    }
    for (int r = 1; r <= n; r ++)
    {
        for (const Query &q : Vec[r])
        {
            Find(q.l);
            Ans[q.id] = (B[q.l].Residual(q.x) == 0);
        }
        if (r < n) Fa[r] = r + 1;
    }
    for (int i = 1, l, r, x; i <= m; i ++)
        puts(Ans[i] ? "YES" : "NO");
    return 0;
}
