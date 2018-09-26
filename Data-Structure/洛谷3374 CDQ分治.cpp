/*
 * @Samaritan_infi
 */
/// 题意: 1 pos val 单点修改， 2 l r 区间求和
/// 初始值也视为修改操作，左修改对右求和产生影响，分治
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

const int N = 5e6 + 5;
struct node {
    int id, type; LL val;
    node () {}
    node (int a, int b, int c)
    : id(a), type(b), val(c) {}
    bool operator < (const node & k) const {
        return id == k.id ? type < k.type : id < k.id;
    }
};
node arr[N], brr[N];
LL ans[N];
/// CDQ分治，必须离线处理，常数大，只考虑左块对右块的影响，然后排序
void CDQ (int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    CDQ(l, mid); CDQ(mid + 1, r);
    int cur1 = l, cur2 = mid + 1; LL sum = 0; /// sum 为修改的总值
    for (int i = l; i <= r; i++) {
        if (cur1 <= mid && arr[cur1] < arr[cur2] || cur2 > r) {
            if (arr[cur1].type == 1) sum += arr[cur1].val;
            brr[i] = arr[cur1++];
        } else {
            if (arr[cur2].type == 3) ans[ arr[cur2].val ] += sum;
            else if (arr[cur2].type == 2) ans[ arr[cur2].val ] -= sum;
            brr[i] = arr[cur2++];
        }
    }
    for (int i = l; i <= r; i++) arr[i] = brr[i];
}

int main () {
    int n, m; scanf("%d %d", &n, &m);
    int query_num = 0, tot = 0, x, l, r;
    for (int i = 1; i <= n; i++) { /// 初始值当作修改操作 type = 1
        scanf("%d", &x);
        arr[++tot] = node (i, 1, x);
    }
    LL val;
    for (int i = 1; i <= m; i++) {
        scanf("%d", &x);
        if (x == 1) {
            scanf("%d %lld", &l, &val); /// 第 l 个数加上 r, 修改
            arr[++tot] = node (l, x, val);
        } else {
            scanf("%d %d", &l, &r);    /// 求 l 到 r 的区间和
            arr[++tot] = node (l - 1, 2, ++query_num);
            arr[++tot] = node (r, 3, query_num);
        }
    }
    CDQ(1, tot);
    for (int i = 1; i <= query_num; i++) printf("%lld\n", ans[i]);
    return 0;
}
