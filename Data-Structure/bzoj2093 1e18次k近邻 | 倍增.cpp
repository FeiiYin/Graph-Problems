/*
 * @Samaritan_infi
 */
/// 题意: 给定一条线上n个不重复的点，每次跳到第k近的点，问m(1e18)轮后各个点跳到哪里
/// 题解: 扫一遍获得每个点的k近的点，倍增，由于会报内存，所以每次复制
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e6 + 5;

LL dis[N];
int k_neighbour[N]; /// 表示第 k 近的石头
int temp[N], ans[N]
int main() {
    LL n, k, m; scanf("%lld %lld %lld", &n, &k, &m);
    for (int i = 1; i <= n; ++i) scanf("%lld", &dis[i]);
    int l = 1, r = k + 1;
    k_neighbour[1] = r;
    for (int i = 2; i <= n; ++i) {
        while (r < n && dis[i] - dis[l] > dis[r + 1] - dis[i])
            l++, r++;
        if (dis[i] - dis[l] < dis[r] - dis[i]) k_neighbour[i] = r;
        else k_neighbour[i] = l;
    }
    for (int i = 1; i <= n; ++i) ans[i] = i;
    while (m) { /// 倍增，类似快速幂
        if (m & 1) {
            for (int i = 1; i <= n; ++i)
                temp[i] = k_neighbour[ans[i]];
            for (int i = 1; i <= n; ++i)
                ans[i] = temp[i];
        }
        for (int i = 1; i <= n; ++i)
            temp[i] = k_neighbour[ k_neighbour[i] ];
        for (int i = 1; i <= n; ++i)
            k_neighbour[i] = temp[i];
        m >>= 1;
    }
    for (int i = 1; i <= n; ++i) printf("%d ", ans[i]);
    return 0;
}
