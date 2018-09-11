/*
 * @Samaritan_infi
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int maxn = 64;
int N, M, t, T, v[maxn];
struct Mat
{
    LL A[maxn][maxn];
    void Print() const
    {
        for(int i = 0;i < N;++i)
            for(int j = 0;j < N;++j) printf("%lld%c", A[i][j], " \n"[j+1 == N]);
        puts("");
    }
};

Mat operator + (const Mat &a, const Mat &b)
{
    Mat ans;
    for(int i = 0;i < N;++i) for(int j = 0;j < N;++j)
        ans.A[i][j] = max(a.A[i][j], b.A[i][j]);
    return ans;
}

Mat operator * (const Mat &a, const Mat &b)
{
    Mat ans;
    for(int i = 0;i < N;++i) for(int j = 0;j < N;++j)
    {
        ans.A[i][j] = max(a.A[i][j], b.A[i][j]);
        for(int k = 0;k < N;++k)
        {
            if(a.A[i][k] && b.A[k][j])
                ans.A[i][j] = max(ans.A[i][j], a.A[i][k] + b.A[k][j] - v[k]); /// 两条边实际上是3个点的权值
        }
    }
    return ans;
}

Mat G, I;
void init()
{
    scanf("%d%d%d%d", &N, &M, &t, &T);
    int x, y;
    while(M--)
    {
        scanf("%d%d", &x, &y);
        --x, --y;
        G.A[x][y] = 1;
    }
    for(int i = 0;i < N;++i) scanf("%d", &v[i]);
    for(int i = 0;i < N;++i) for(int j = 0;j < N;++j)
    {
        if(G.A[i][j] == 1) G.A[i][j] = v[i] + v[j];
    }
    for(int i = 0;i < N;++i) for(int j = 0;j < N;++j)
        I.A[i][j] = 0;
    for(int i = 0;i < N;++i)
        I.A[i][i] = v[i];
}

Mat powmod(Mat a, int n)
{
    Mat ans = I;
    while(n)
    {
        if(n & 1) ans = ans * a;
        a = a * a;
        n >>= 1;
    }
    return ans;
}

Mat powmod2(Mat a, Mat g, int n)
{
   /// 把 G 放到连续的 t 之间 在每一轮开始的时候，a都相当于“在g的任意位置插入血辣段”得到的最大情况
  /// 然后pw相当于正常的快速幂，且在每轮它的幂次一定小于a的幂次
///所以pw*a+a*pw覆盖了当前长度所有的情况，就是说你考虑把当前需要的长度拆成一
///大一小的两份然后交换一下位置，那么把较大的那份的所有情况全考虑进去就可以考虑到新的长度的所有情况了
/// 改成直接求出来不大于n的最大的2^k，然后以这个为“较长的段”就好了
    Mat ans = a, pw = I;
    a = a * g + g * a;
    while(n)
    {
        if(n & 1)
        {
            ans = ans + pw * a + a * pw;
            pw = pw * g + g * pw;
        }
        n >>= 1;
        a = a * g + g * a;
        g = g * g;
    }
    return ans;
}

void work()
{
    Mat a = powmod(G, T-1); /// T-1 条边, T个点
    bool useXL = false;
    for(int i = 0;i < N;++i) for(int j = 0;j < N;++j)
        if(a.A[i][j])
        {
            useXL = true;
            a.A[i][j] <<= 1;
        }
    LL ans = 0;
    if(useXL)
    {
        a = powmod2(a, G, t - T);
        for(int i = 0;i < N;++i) for(int j = 0;j < N;++j) ans = max(ans, a.A[i][j]);
    }

    a = powmod(G, t);
    for(int i = 0;i < N;++i) for(int j = 0;j < N;++j) ans = max(ans, a.A[i][j]);
    printf("%lld\n", ans);
}

int main()
{
    init();
    work();
    return 0;
}

