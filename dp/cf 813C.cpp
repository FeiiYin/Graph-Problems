/// 给出长度为n的序列，从中找出2个子序列，满足每个子序列相邻两数之间要么相差1，要么同余于7，求这两个子序列的最长长度和。
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
    LL ans = 1LL, base = a;
    while (b) {
        if (b & 1LL) (ans *= base) %= mod;
        b >>= 1LL;
        (base *= base) %= mod;
    }
    return ans;
}
void gcd(LL a, LL b, LL &d, LL &x, LL &y) {
    if (!b) {
        d = a; x = 1LL; y = 0;
    } else {
        gcd(b, a % b, d, y, x);
        y -= x * (a / b);
    }
}
LL get_inv(LL a, LL n) {
    LL d, x, y;
    gcd(a, n, d, x, y);
    return d == 1LL ? (x + n) % n : -1LL;
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
const int mod = 7;
const int N = 5e3 + 5;
const int M = 1e5 + 5;
const int logN = 20;
const int INF = 0x3f3f3f3f;
int arr[N];
inline bool same(int a, int b) {
    if (abs(a - b) <= 1) return true;
    return (!a && b == 6) || (a == 6 && !b);
}
/// dp[i][j] 两个串以 i, j 结尾和的最大长度
/// dp[i][j] = dp[j][i]
/// dp[i][j] = same(j, k) -> update: dp[i][k] + 1
/// 维护取模下前驱最大即可快速更新
int dp[N][N];
int premod[10], pre[M];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &arr[i]);
    }

    int ans = 1;
    /// i 从 0 开始，因为 0 的时候表示单独串进行计算
    /// j 从 1 开始，因为不存在 arr[0]，会计算错误
    for (int i = 0; i <= n; ++i) {
        memset(premod, 0, sizeof premod);
        memset(pre, 0, sizeof pre);
        for (int j = 1; j < i; ++j) {
            /// premod[arr[k]] = max(dp[i][k])
            premod[arr[j] % mod] = max(premod[arr[j] % mod], dp[i][j]);
            pre[arr[j]] = max(pre[arr[j]], dp[i][j]);
        }
        for (int j = i + 1; j <= n; ++j) {
            dp[i][j] = max(pre[arr[j] + 1] + 1, pre[arr[j] - 1] + 1);
            dp[i][j] = max(dp[i][j], premod[arr[j] % mod] + 1);
            /// 直接取该位为第一位
            dp[i][j] = max(dp[i][j], dp[i][0] + 1);

            dp[j][i] = dp[i][j];
            premod[arr[j] % mod] = max(premod[arr[j] % mod], dp[i][j]);
            pre[arr[j]] = max(pre[arr[j]], dp[i][j]);
            ans = max(ans, dp[i][j]);
        }
    }
//    for (int i = 0; i <= n; ++i) {
//        for (int j = 0; j <= n; ++j) {
//            cout << dp[i][j] << " ";
//        } cout << endl;
//    }
    printf("%d\n", ans);
    return 0;
}
