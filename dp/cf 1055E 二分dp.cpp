/// 题意：给出一个序列和一些线段，要求恰好选出mmm条线段覆盖序列，求被覆盖数从小到大排序后第kkk个的最小值。
/// 首先可以二分出这个值，当然由于值是固定的，所以可以离散原来的权值后，对每个位置进行二分，这样就是log(1500)了
/// check 时只要满足 dp[n][m] 即总共m个区间覆盖n个数能使小于二分值的数大于k，即当前值能成立
/// 然后我们去枚举每一个位置结尾和每一个区间结尾，
/// 位置结尾，显然符合前缀max的性质
/// 区间结尾，即通过当前区间结尾到当前位置结尾之间的小于个数进行更新，这个可以用类似前缀和进行快速维护
/// 然后就是 O(n*n*logn) 完成了
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
#define PIL pair<int, LL>

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
struct node {
    int dist, id;
    node() {
    }
    node(int d, int i) : dist(d), id(i) {}
    bool operator < (const node& k) const {
        if (dist == k.dist)
            return id <= k.id;
        return dist < k.dist;
    }
};

const int N = 2e3 + 5;
const int INF = 0x3f3f3f3f;

int arr[N];
int l[N], r[N];
int max_r[N];
int dp[N][N];
int pre_sum[N];
int m, s, n, k;
vector<int> val;
bool check(int mid) {
    memset(dp, 0, sizeof dp);
    pre_sum[0] = 0;
    for (int i = 0; i < n; ++i) pre_sum[i + 1] = pre_sum[i] + (arr[i] <= val[mid]);
//    cout << "pre_sum: " << endl;
//    for (int i = 0; i <= n; ++i) cout << pre_sum[i] << " "; cout << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= s; ++j) {
            dp[i + 1][j] = max(dp[i + 1][j], dp[i][j]);
            if (j < s && max_r[i] > i)
                dp[max_r[i]][j + 1] = max(dp[max_r[i]][j + 1], dp[i][j] + (pre_sum[max_r[i]] - pre_sum[i]));
        }
    }
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j <= s; ++j) {
//            cout << dp[i][j] << " ";
//        } cout << endl;
//    }
    return dp[n][m] >= k;
}
int main() {
    scanf("%d %d %d %d", &n, &s, &m, &k);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
        val.pb(arr[i]);
    }
    sort(val.begin(), val.end());
    val.resize(unique(val.begin(), val.end()) - val.begin());
    for (int i = 0; i < s; ++i) {
        scanf("%d %d", &l[i], &r[i]);
//        l[i]--;
        for (int j = l[i] - 1; j < r[i]; ++j)
            max_r[j] = max(max_r[j], r[i]);
    }
    int L = 0, R = val.size();
    while (L < R) {
        int mid = (L + R) >> 1;
        if (check(mid)) R = mid;
        else L = mid + 1;
    }
    if (R == val.size()) puts("-1");
    else printf("%d\n", val[R]);
    return 0;
}
