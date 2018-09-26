/*
 * @Samaritan_infi
 */
/// 题意: 平面内, 操作 1 x y z (x, y) 增加了z个单位
///               操作 2 x1 y1 x2 y2  查询矩形内单位个数
/// 根据y坐标建立树状数组，x坐标通过排序保证递增，排序注意把修改操作放前面，即看该点修改是否影响该点
/// 注意查询是 (x1-1, x2) (y1-1, y2) 以及注意清空BIT， 分治
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

const int N = 2e5 + 5, TREE_SIZE = 5e5 + 5;
struct node {
    int id, type, val;
    int x1, y1, x2, y2;
    node () {}
    node (int _id, int _t, int _x, int _y, int _z)
    : id(_id), type(_t), x1(_x), y1(_y), val(_z) {}
    node (int _id, int _t, int _x1, int _y1, int _x2, int _y2)
    : id(_id), type(_t), x1(_x1), y1(_y1), x2(_x2), y2(_y2) {val = 0;}
    bool operator < (const node & k) const {
        return x1 == k.x1 ? type < k.type : x1 < k.x1;
    }
};
node arr[N], brr[N];
/// BIT
int tree[TREE_SIZE], tree_top;
inline int low_bit (int x) { return x & (-x); }
void add (int x, int val) {
    for (; x <= tree_top; x += low_bit(x)) tree[x] += val;
}
int query (int x) {
    int ans = 0;
    for (; x; x -= low_bit(x)) ans += tree[x];
    return ans;
}
/// CDQ分治，必须离线处理，常数较大，只考虑左块对右块的影响，然后排序
/// 三维偏序问题 套树状数组
void CDQ (int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1, cnt = 0;
    CDQ(l, mid); CDQ(mid + 1, r);
    /// 只考虑左边修改影响右边的查询
    for (int i = l; i <= mid; i++) if (arr[i].type == 1) brr[cnt++] = arr[i];
    for (int i = mid + 1; i <= r; i++) if (arr[i].type == 2) {
        brr[cnt++] = arr[i]; brr[cnt-1].x1 --;
        brr[cnt++] = arr[i]; brr[cnt-1].type = 3;
        brr[cnt-1].x1 = arr[i].x2;
    }
    sort (brr, brr + cnt);  /// 降维
    for (int i = 0; i < cnt; i++) {
        if (brr[i].type == 1) add(brr[i].y1, brr[i].val);
        else if (brr[i].type == 2) arr[ brr[i].id ].val -= query(brr[i].y2) - query(brr[i].y1-1);
        else arr[ brr[i].id ].val += query(brr[i].y2) - query(brr[i].y1-1);
    }
    for (int i = 0; i < cnt; i++) if (brr[i].type == 1)
        add(brr[i].y1, -brr[i].val); /// 清空BIT
}
int main () {
    //freopen("locust.in","r",stdin); cogs oj 需要加freopen = =
	//freopen("locust.out","w",stdout);
    int n; scanf("%d %d", &tree_top, &n);
    int x1, y1, x2, y2, val, type;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &type);
        if (type == 1) { /// 插入
            scanf("%d %d %d", &x1, &y1, &val);
            arr[i] = node (i, type, x1, y1, val);
        } else { /// 查询
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            arr[i] = node (i, type, min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
        }
    }
    CDQ(1, n);
    for (int i = 1; i <= n; i++) if (arr[i].type == 2) printf("%d\n", arr[i].val);
    return 0;
}
