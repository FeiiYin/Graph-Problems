/*
 * @Samaritan_infi
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N = 1e5 + 5;
int arr[N];

/// ST表 预处理O(nlogn) 查询时间 O(1)
const int ST_N = 200000;
int Bin[20], Log[ST_N];
int mn[20][ST_N];/// mn[i][j]表示从j到j+2^i-1的最小值
void ST (int n) {
    Bin[0] = 1;
    for (int i = 1; i < 20; i ++)
        Bin[i] = Bin[i - 1] << 1;
    /// 查询多的时候可以预处理Log
//    Log[0] = -1;
//    for (int i = 1; i <= N; i ++)
//        Log[i] = Log[i / 2] + 1;
    for (int i = 1; i <= n; i ++)
        mn[0][i] = arr[i];
    int k = int (log2(n));
    for (int i = 1; i <= k; i ++)
        for (int j = 1; j + Bin[i] - 1 <= n; j ++)
            mn[i][j] = max(mn[i - 1][j], mn[i - 1][j + Bin[i - 1]]);
}
/// 查询
/// int t=Log[y-x+1];
/// min(mn[t][x],mn[t][y-bin[t]+1])

/// codeforces 601B 求 所有区间的 max( abs[al-ar]/(l-r))
/// 发现性质 l 和 r 之间的差一定是 1
int right_max[N], left_max[N];
void init_LR (int n) {
    int k = int (log2(n));
    int temp;
    /// 预处理向左向右最长小于当前位置的长度
    /// 即在这段区间包含它的选取最大值就是选取它
    /// 即arr[i] 的贡献次数是 (left + 1)*(right + 1)
    /// 枚举区间左右端点
    for (int i = 1; i < n - 1; i ++) {
        temp = i + 1;
        for (int j = k; j >= 0; j --)
            if (temp + (1 << j) - 1< n && mn[j][temp] <= arr[i])
                right_max[i] += 1 << j, temp += 1 << j;
    }
    for (int i = 2; i < n; i ++) {
        temp = i - 1;
        for (int j = k; j >= 0; j --)
            if (temp - (1 << j) + 1 >= 1 && mn[j][temp - (1 << j) + 1] < arr[i])
            left_max[i] += 1 << j, temp -= 1 << j;
    }
}
int seq[N];
int main () {
    int n, q; scanf("%d %d", &n, &q);
    for (int i = 1; i <= n; i ++)
        scanf("%d", &seq[i]);
    for (int i = 1; i < n; i ++)
        arr[i] = abs(seq[i + 1] - seq[i]);
    ST(n);
    init_LR(n);
    ll ans = 0;
    int l, r;
    while (q --) {
        scanf("%d %d", &l, &r); ans = 0;
        for (int i = l; i < r; i ++)
            ans += 1LL * (min(left_max[i], i - l) + 1) * (min(right_max[i], r - i - 1) + 1) * arr[i] ;
        printf("%lld\n", ans);
    }
    return 0;
}

/*  ST表
    初始区间更新最大值，最后查询
    if(L > R)
        swap(L, R);
    int d = Log[R - L + 1];
    upd(a[d][L], v);
    upd(a[d][R - (1 << d) + 1], v);


    for(int i = mx - 1; i > 0; --i)
        for(int j = 1; j + (1 << i) - 1 <= n; ++j) {
            upd(a[i - 1][j], a[i][j]);
            upd(a[i - 1][j + (1 << (i - 1))], a[i][j]);
            a[i][j] = 0;
        }
*/
