/*
 * @Samaritan_infi
 */
/// 题意: 区间内所有子段不同GCD值的个数
/// 题解: 类似于求区间不同数的个数，先求出固定右端点，gcd的值和左端点,因为是阶梯的，扫一遍即可
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e6 + 5;
int arr[N], ans[N], last_pos[N];
vector< pair<int, int> > gg[N]; /// 固定右端点为 i 的 gcd 的值
struct node {
    int l, r, id;
    node () {}
    node (int l, int r, int id) : l(l), r(r), id(id) {}
    bool operator < (const node & k) const {
        return r < k.r;
    }
};
node que[N];
inline int low_bit (int x) { return x & (-x); }
int tree[N];
void add (int pos, int val, int n) {
    for (; pos <= n; pos += low_bit(pos)) tree[pos] += val;
}
int query (int pos) {
    int ans = 0;
    for (; pos > 0; pos -= low_bit(pos)) ans += tree[pos];
    return ans;
}
int main () {
    //freopen("in.txt", "r", stdin);
    int n, q; while (~ scanf("%d %d", &n, &q)) {
        for (int i = 1; i <= n; ++i) {
            scanf("%d", &arr[i]);
            gg[i].clear();
        }
        memset(tree, 0, sizeof tree);
        memset(last_pos, 0, sizeof last_pos);
        for (int i = 1; i <= n; ++i) {
            int x = arr[i], y = i;
            for (int j = 0; j < gg[i - 1].size(); ++j) {
                int gcd = __gcd(gg[i - 1][j].first, x);
                if (x != gcd) {
                    gg[i].push_back(make_pair(x, y));
                    x = gcd, y = gg[i - 1][j].second;
                }
            }
            gg[i].push_back(make_pair(x, y));
//            cout <<" i : " << i << endl;
//            for (int j = 0; j < gg[i].size(); j++) cout << gg[i][j].first << " " << gg[i][j].second << endl;
        }
        int l, r;
        for (int i = 0; i < q; ++i) {
            scanf("%d %d", &l, &r);
            que[i] = node(l, r, i);
        }
        sort (que, que + q);
        int cur = 0;
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j < gg[i].size(); ++j) {
                int gcd = gg[i][j].first, pos = gg[i][j].second;
                if (last_pos[gcd]) add(last_pos[gcd], -1, n);
                add(pos, 1, n);  last_pos[gcd] = pos;
            }
            while (que[cur].r == i) {
                ans[que[cur].id] = query(que[cur].r) - query(que[cur].l - 1);
                cur++;
            }
        }
        for (int i = 0; i < q; i ++) printf("%d\n", ans[i]);
    }
    return 0;
}
