/*
 * @Samaritan_infi
 */
/// 题意: 给出一个数组，每次查询一个区间的gcd,和[1, n]区间中有多少个gcd等于它的区间
/// 题解: 对于区间gcd通过st表维护查询即可, 对于每个gcd的个数的计算,通过固定区间左端点,
///      此时，区间的gcd是呈阶梯分布的，所以二分最后一段不变的分界线坐标为pos
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

const int N = 1e5 + 5;
int _gcd(int a, int b) { return b ? _gcd(b, a % b) : a; }
int arr[N], gcd[N][30];
map <int, LL> mp;
int query (int l, int r) { /// 查询区间 l, r 的 gcd
    int len = r - l + 1;
    int temp = (int)log2(double(len));
    return _gcd(gcd[l][temp], gcd[r - (1<<temp)+1][temp]);
}
int query_number (int l, int r) {
    int g = query(l, r), L = l, R = r, mid;
    while (L != R) {
        mid = (L + R) >> 1;
        if (query(L, mid) <= g) R = mid;
        else                    L = mid + 1;
    }
    mp[g] += (r - L + 1);
    return L;
}
void cal_interval (int n) {
    mp.clear();
    for (int i = 1; i <= n; ++i) {
        int r = n, pos = query_number(i, r);
        while (pos != i) {
            r = pos - 1;
            pos = query_number(i, r);
        }
    }
}
int main () {
    int kase; scanf("%d", &kase); int _ = 1;
    while (kase --) {
        int n; scanf("%d", &n);
        for (int i = 1; i <= n; ++i) {
            scanf("%d", &arr[i]);
            gcd[i][0] = arr[i];
        }
        int nl = (int)log2(double(n));
        /// st 表 | -- | -- | 类似倍增，下面两段的gcd
        for (int j = 1; j <= nl; ++j)
            for (int i = 1; i <= n; ++i) if (i + (1<<(j-1)) <= n) {
                gcd[i][j] = _gcd(gcd[i][j-1], gcd[i + (1<<(j-1))][j-1]);
            }
        cal_interval(n);
        int op; scanf("%d", &op); printf("Case #%d:\n", _++);
        while (op --) {
            int l, r; scanf("%d %d", &l, &r);
            int gcd = query(l, r);
            printf("%d %lld\n", gcd, mp[gcd]);
        }
    }
    return 0;
}
