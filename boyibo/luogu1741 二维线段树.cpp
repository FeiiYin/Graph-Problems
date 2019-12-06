/// 每次二维修改一个值，最后查询全局的最大值
/**
 * @Samaritan
 */
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;

#define pb emplace_back
#define fi first
#define se second
#define PII pair<int, int>
#define PLL pair<LL, LL>
#define PLI pair<LL, int>

LL quick_pow (LL a, LL b, LL mod) {
    LL ans = 1, base = a;
    while (b) {
        if (b & 1) (ans *= base) %= mod;
        b >>= 1;
        (base *= base) %= mod;
    }
    return ans;
}
void gcd(LL a, LL b, LL &d, LL &x, LL &y) {
    if (!b) {
        d = a; x = 1; y = 0;
    } else {
        gcd(b, a % b, d, y, x);
        y -= x * (a / b);
    }
}
LL get_inv(LL a, LL n) {
    LL d, x, y;
    gcd(a, n, d, x, y);
    return d == 1 ? (x + n) % n : -1;
}
inline int log2(const float &x) {
    return ((unsigned &) x >> 23 & 255) - 127;
}
template <typename type>
void show(type arr[], int len) {
    for (int i = 0; i <= len; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
const int mod = 998244353;
const int N = 1e3 + 5;
const int logN = 20;
const int INF = 0x3f3f3f3f;

struct segment_tree_x{
    int tree[N << 2], lazy[N << 2];
    inline void push_up(int rt) {
        if (tree[rt<<1])
            tree[rt] = max(tree[rt<<1], tree[rt]);
        if (tree[rt<<1|1])
            tree[rt] = max(tree[rt<<1|1], tree[rt]);
    }
    inline void push_down(int rt) {
        if (lazy[rt]) {
            lazy[rt<<1|1] = lazy[rt<<1] = lazy[rt];
            tree[rt<<1|1] = tree[rt<<1] = lazy[rt];
            lazy[rt] = 0;
        }
    }
//    void update(int rt, int l, int r, int L, int R, int val) {
//        if (L <= l && r <= R) {
//            tree[rt] = max(tree[rt], val);
//            lazy[rt] = max(lazy[rt], val);
//            return;
//        }
//        push_down(rt);
//        int mid = (l + r) >> 1;
//        if (L <= mid) update(rt<<1, l, mid, L, R, val);
//        if (R > mid) update(rt<<1|1, mid+1, r, L, R, val);
//        push_up(rt);
//    }
    // 开tag标记而不适用push_down
    void update(int rt, int l, int r, int L, int R, int val) {
        tree[rt] = max(tree[rt], val);
        if (L <= l && r <= R) {
            lazy[rt] = max(lazy[rt], val);
            return;
        }
        int mid = (l + r) >> 1;
        if (L <= mid) update(rt<<1, l, mid, L, R, val);
        if (R > mid) update(rt<<1|1, mid+1, r, L, R, val);
    }
    int query(int rt, int l, int r, int L, int R) {
        if (L <= l && r <= R) return tree[rt];
        int mid = (l + r) >> 1;
        int res = lazy[rt];
        if (L <= mid) res = max(res, query(rt<<1, l, mid, L, R));
        if (R > mid) res = max(res, query(rt<<1|1, mid+1, r, L, R));
        return res;
    }
};
int length, width;
int cube_down, cube_up, cube_left, cube_right;
/// 每一个y建一颗线段树
struct segment_tree_y{
    segment_tree_x v[N << 2], lazy[N << 2];
    void update(int rt, int l, int r, int L, int R, int val) {
        v[rt].update(1, 1, width, cube_left, cube_right, val);
        if (L <= l && r <= R) {
            lazy[rt].update(1, 1, width, cube_left, cube_right, val);
            return;
        }
        int mid = (l + r) >> 1;
        if (L <= mid) update(rt<<1, l, mid, L, R, val);
        if (R > mid) update(rt<<1|1, mid+1, r, L, R, val);
    }
    int query(int rt, int l, int r, int L, int R) {
        if (L <= l && r <= R) return v[rt].query(1, 1, width, cube_left, cube_right);
        int mid = (l + r) >> 1;
        int res = lazy[rt].query(1, 1, width, cube_left, cube_right);
        if (L <= mid) res = max(res, query(rt<<1, l, mid, L, R));
        if (R > mid) res = max(res, query(rt<<1|1, mid+1, r, L, R));
        return res;
    }
} T;
int main() {
    int n;
    scanf("%d %d %d", &length, &width, &n);
    int cube_length, cube_width, cube_height, x, y;
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d %d %d", &cube_length, &cube_width, &cube_height, &y, &x);
        cube_down = y + 1;
        cube_up = y + cube_length;
        cube_left = x + 1;
        cube_right = x + cube_width;
        /// 在最大值上直接加
        int ans = T.query(1, 1, length, cube_down, cube_up);
        T.update(1, 1, length, cube_down, cube_up, ans + cube_height);
    }
    cube_left = 1; cube_right = width;
    cube_down = 1; cube_up = length;
    printf("%d\n", T.query(1, 1, length, cube_down, cube_up));
    return 0;
}
