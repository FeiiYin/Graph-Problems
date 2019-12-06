///
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
const int N = 5e4 + 5;
const int logN = 20;
const int INF = 0x3f3f3f3f;

const int MAXN = 1e5 + 100;
int n, m;
int a[MAXN];

namespace Treap {
    struct balanced {
        int w, sz, num, fix, ch[2];
    };
    int tot;
    balanced tree[MAXN * 20];
    int newnode(int w) {
        ++tot;
        tree[tot].w = w;
        tree[tot].fix = rand();
        tree[tot].num = 1;
        tree[tot].ch[0] = tree[tot].ch[1] = 0;
        tree[tot].sz = 1;
        return tot;
    }
    void pushup(int p) {
        tree[p].sz = tree[tree[p].ch[0]].sz + tree[tree[p].ch[1]].sz + tree[p].num;
    }
    void rotate(int &p, int d) {
        int y = tree[p].ch[d];
        tree[p].ch[d] = tree[y].ch[d ^ 1];
        tree[y].ch[d ^ 1] = p;
        pushup(p);
        pushup(y);
        p = y;
    }
    void insert(int &p, int w) {
        if (!p) p = newnode(w);
        else if (tree[p].w == w) ++tree[p].num;
        else {
            if (tree[p].w > w) {
                insert(tree[p].ch[0], w);
                if (tree[tree[p].ch[0]].fix > tree[p].fix)
                    rotate(p, 0);
            } else {
                insert(tree[p].ch[1], w);
                if (tree[tree[p].ch[1]].fix > tree[p].fix)
                    rotate(p, 1);
            }
        }
        pushup(p);
    }
    void remove(int &p, int w) {
        if (tree[p].w > w) remove(tree[p].ch[0], w);
        else if (tree[p].w < w) remove(tree[p].ch[1], w);
        else {
            if (tree[p].num > 1) --tree[p].num;
            else {
                if (!tree[p].ch[0] && !tree[p].ch[1]) p = 0;
                else if (!tree[p].ch[0]) {
                    rotate(p, 1);
                    remove(tree[p].ch[0], w);
                } else if (!tree[p].ch[1]) {
                    rotate(p, 0);
                    remove(tree[p].ch[1], w);
                } else {
                    if (tree[tree[p].ch[0]].fix > tree[tree[p].ch[1]].fix) {
                        rotate(p, 0);
                        remove(tree[p].ch[1], w);
                    } else {
                        rotate(p, 1);
                        remove(tree[p].ch[0], w);
                    }
                }
            }
        }
        if (p) pushup(p);
    }
    int queryrank(int p, int k) {// return the highest rank of value 'k'
        if (!p) return 0;
        if (tree[p].w > k) return queryrank(tree[p].ch[0], k);
        else if (tree[p].w == k) return tree[tree[p].ch[0]].sz;
        else return tree[tree[p].ch[0]].sz + tree[p].num + queryrank(tree[p].ch[1], k);
    }
    int querynum(int p, int k) {// return the value of kth rank node
        if (tree[tree[p].ch[0]].sz + 1 == k) return tree[p].w;
        else if (tree[tree[p].ch[0]].sz + 1 < k) return querynum(tree[p].ch[1], k - 1 - tree[tree[p].ch[0]].sz);
        else return querynum(tree[p].ch[0], k);
    }
    int querypre(int p, int k) {// return the prefix of value k
        if (!p) return -2147483647;
        if (tree[p].w >= k) return querypre(tree[p].ch[0], k);
        else return max(tree[p].w, querypre(tree[p].ch[1], k));
    }
    int querysuf(int p, int k) {// return the suffix of value k
        if (!p) return 2147483647;
        if (tree[p].w <= k) return querysuf(tree[p].ch[1], k);
        else return min(tree[p].w, querysuf(tree[p].ch[0], k));
    }
    void listall(int p) {
        if (tree[p].ch[0]) listall(tree[p].ch[0]);
        cerr << tree[p].w << ",sz=" << tree[p].num << "   ";
        if (tree[p].ch[1]) listall(tree[p].ch[1]);
    }
}

namespace SEG {
    struct segment {
        int l, r, root;
    };
    segment tree[MAXN * 8];
    void build(int rt, int l, int r) {
        tree[rt].l = l;
        tree[rt].r = r;
        for (int i = l; i <= r; ++i) {
            /// treap insert 对于新插入的会动态新建节点
            /// segment tree 上每一个节点都有一棵树
            Treap::insert(tree[rt].root, a[i]);
        }
        if (l != r) {
            int mid = (l + r) >> 1;
            build(rt << 1, l, mid);
            build(rt<<1|1, mid + 1, r);
        }
    }
    void single_modify(int rt, int pos, int change_val) {
        Treap::remove(tree[rt].root, a[pos]);
        Treap::insert(tree[rt].root, change_val);
        if (tree[rt].l == tree[rt].r) return;
        int mid = (tree[rt].l + tree[rt].r) >> 1;
        if (pos <= mid) single_modify(rt<<1, pos, change_val);
        else single_modify(rt<<1|1, pos, change_val);
    }
    /// query the highest rank of value 'k' 表示查询值k在区间[L,R]的排名
    int query_rank(int rt, int L, int R, int k) {
        if (tree[rt].l > R || tree[rt].r < L) return 0;
        if (L <= tree[rt].l && tree[rt].r <= R) return Treap::queryrank(tree[rt].root, k);
        else return query_rank(rt<<1, L, R, k) + query_rank(rt<<1|1, L, R, k);
    }
    ///  表示查询区间[l,r]内排名为k的数
    int query_num(int L, int R, int k) {
        int l = 0, r = 1e8; // 数的max
        while (l < r) {
            int mid = (l + r + 1) / 2;
            if (query_rank(1, L, R, mid) < k) l = mid;
            else r = mid - 1;
        }
        return r;
    }
    /// 表示查询区间[l,r]内k的前驱, 严格小于k，且最大的数，若不存在输出-2147483647
    int query_pre(int rt, int L, int R, int k) {
        if (tree[rt].l > R || tree[rt].r < L) return -2147483647;
        if (L <= tree[rt].l && tree[rt].r <= R) return Treap::querypre(tree[rt].root, k);
        else return max(query_pre(rt<<1, L, R, k), query_pre(rt<<1|1, L, R, k));
    }
    /// 表示查询区间[l,r]内k的后继, 严格大于x，且最小的数，若不存在输出2147483647
    int query_suf(int rt, int L, int R, int k) {
        if (tree[rt].l > R || tree[rt].r < L) return 2147483647;
        if (L <= tree[rt].l && tree[rt].r <= R) return Treap::querysuf(tree[rt].root, k);
        else return min(query_suf(rt<<1, L, R, k), query_suf(rt<<1|1, L, R, k));
    }
}
int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i)
        scanf("%d", &a[i]);
    SEG::build(1, 1, n);
    for (int i = 0; i < m; ++i) {
        int opt; scanf("%d", &opt);
        if (opt == 3) {
            /// 单点修改
            int pos, y;
            scanf("%d %d", &pos, &y);
            SEG::single_modify(1, pos, y);
            a[pos] = y;
        } else {
            int l, r, k;
            scanf("%d %d %d", &l, &r, &k);
            if (opt == 1) ///  表示查询k在区间[l,r]的排名
                printf("%d\n", SEG::query_rank(1, l, r, k) + 1);
            else if (opt == 2) ///  表示查询区间[l,r]内排名为k的数
                printf("%d\n", SEG::query_num(l, r, k));
            else if (opt == 4) /// 表示查询区间[l,r]内k的前驱, 严格小于k，且最大的数，若不存在输出-2147483647
                printf("%d\n", SEG::query_pre(1, l, r, k));
            else if (opt == 5) /// 表示查询区间[l,r]内k的后继, 严格大于x，且最小的数，若不存在输出2147483647
                printf("%d\n", SEG::query_suf(1, l, r, k));
        }
    }
    return 0;
}
