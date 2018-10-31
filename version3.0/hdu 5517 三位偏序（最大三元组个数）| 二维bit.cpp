/*
 * @Samaritan_infi
 */
/// 题意: 化简原串成为一个三元组，求有多少个三元组不存在所有维都大于当前组的个数
/// 题解: 使用二维BIT，先按照第一维排序，然后按二三维度查询，再更新。
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e5 + 5;
const int M = 1e3 + 5;

struct node {
    int a, b, c, num;
    node () {}
    node (int _a, int _b, int _c, int _num)
    : a(_a), b(_b), c(_c), num(_num) {}
    bool operator < (const node &k) const {
        if (a != k.a) return a < k.a;
        if (b != k.b) return b < k.b;
        if (c != k.c) return c < k.c;
    }
    bool operator == (const node &k) const {
        if (a == k.a && b == k.b && c == k.c) return true;
        return false;
    }
};
int max_pair[N], pair_num[N];
node triple[N], tri[N]; int triple_num, tri_num;

int tree[M][M];
inline int low_bit (int x) { return x & (-x); }
void change (int x, int y, int val) {
    for (int i = x; i < M; i += low_bit(i))
        for (int j = y; j < M; j += low_bit(j))
            tree[i][j] += val;
}
int query (int x, int y) {
    int ans = 0;
    for (int i = x; i; i -= low_bit(i))
        for (int j = y; j; j -= low_bit(j))
            ans += tree[i][j];
    return ans;
} /// 直接查是查比它小的数，插入1001 - value 是插比它大的数
int work () {
    memset(tree, 0, sizeof tree);
    int ans = 0;
    const int block = 1001;
    for (int i = tri_num; i >= 0; --i) {
        if (! query(block - tri[i].b, block - tri[i].c)) ans += tri[i].num;
        change(block - tri[i].b, block - tri[i].c, 1);
    }
    return ans;
}
int main () {
    int kase, _ = 1; scanf("%d", &kase); while (kase--) {
        int n, m; scanf("%d %d", &n, &m);
        memset(max_pair, 0, sizeof max_pair);
        memset(pair_num, 0, sizeof pair_num);
        int x, y, z;
        for (int i = 0; i < n; ++i) {
            scanf("%d %d", &x, &y);
            if (max_pair[y] < x) { max_pair[y] = x, pair_num[y] = 1; }
            else if (max_pair[y] == x) pair_num[y]++;
        }
        triple_num = 0;
        for (int i = 0; i < m; ++i) {
            scanf("%d %d %d", &x, &y, &z);
            if (pair_num[z]) triple[triple_num++] = node (max_pair[z], x, y, pair_num[z]);
        }
        sort (triple, triple + triple_num);
        tri_num = 1; tri[tri_num] = triple[0];
        for (int i = 1; i < triple_num; ++i) {
            if (tri[tri_num] == triple[i])
                tri[tri_num].num += triple[i].num;
            else
                tri[++tri_num] = triple[i];
        }
        printf("Case #%d: %d\n", _++, work());
    }
    return 0;
}
/// 下面是改成CDQ写法 但是常数太大，会T，只是针对当M太大的时候数组开不下的情况
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e5 + 5;
const int M = 1e3 + 5;

struct node {
    int a, b, c, num;
    bool flag;
    node () {}
    node (int _a, int _b, int _c, int _num)
    : a(_a), b(_b), c(_c), num(_num) { flag = true; }
    bool operator < (const node &k) const {
        if (a != k.a) return a > k.a;
        if (b != k.b) return b > k.b;
        if (c != k.c) return c > k.c;
    }
    bool operator == (const node &k) const {
        if (a == k.a && b == k.b && c == k.c) return true;
        return false;
    }
};
int max_pair[N], pair_num[N];
node triple[N], tri[N]; int triple_num, tri_num;

int tree[M];
inline int low_bit (int x) { return x & (-x); }
void change (int x, int val) {
    for (int i = x; i < M; i += low_bit(i))
        tree[i] += val;
}
int query (int x) {
    int ans = 0;
    for (int i = x; i; i -= low_bit(i)) ans += tree[i];
    return ans;
} /// 直接查是查比它小的数，插入1001 - value 是插比它大的数
node brr[N]; /// CDQ

void CDQ (int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    CDQ(l, mid); CDQ(mid + 1, r);
    int cur1 = l, cur2 = mid + 1;
    for (int i = l; i <= r; ++i) {
        if (cur1 <= mid && tri[cur1].b <= tri[cur2].b || cur2 > r) {
            brr[i] = tri[cur1++];
            change(brr[i].c - 1, 1);
        } else {
            brr[i] = tri[cur2++];
            if (query(brr[i].c)) brr[i].flag = false;
        }
    }
    for (int i = l; i <= mid; ++i) change(tri[i].c - 1, -1);
    for (int i = l; i <= r; ++i) tri[i] = brr[i];
}
int main () {
    int kase, _ = 1; scanf("%d", &kase); while (kase--) {
        int n, m; scanf("%d %d", &n, &m);
        memset(max_pair, 0, sizeof max_pair);
        memset(pair_num, 0, sizeof pair_num);
        int x, y, z;
        for (int i = 0; i < n; ++i) {
            scanf("%d %d", &x, &y);
            if (max_pair[y] < x) { max_pair[y] = x, pair_num[y] = 1; }
            else if (max_pair[y] == x) pair_num[y]++;
        }
        triple_num = 0;
        for (int i = 0; i < m; ++i) {
            scanf("%d %d %d", &x, &y, &z);
            if (pair_num[z]) triple[triple_num++] = node (100001 - max_pair[z], 1001 - x, 1001 - y, pair_num[z]);
        }
        sort (triple, triple + triple_num);

        tri_num = 1; tri[tri_num] = triple[0];
        for (int i = 1; i < triple_num; ++i) {
            if (tri[tri_num] == triple[i])
                tri[tri_num].num += triple[i].num;
            else
                tri[++tri_num] = triple[i];
        }
        CDQ(1, tri_num);
        int ans = 0;
        for (int i = 1; i <= tri_num; ++i) {
            if (tri[i].flag) ans += tri[i].num;
            //    cout << tri[i].a << " " << tri[i].b << " " << tri[i].c << endl;
        }
        printf("Case #%d: %d\n", _++, ans);
    }
    return 0;
}
