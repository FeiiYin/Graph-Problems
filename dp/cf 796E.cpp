/// 我们用 dp[i][j][x][y] 表示前i题看了j次，还可以再连续看第一个人x题，第二个人y题的最大做对数
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
const int N = 1e3 + 5;
const int M = 50+ 5;
const int logN = 20;
const int INF = 0x3f3f3f3f;
 
int dp[2][N][M][M];
int a[2][N];
 
inline void get_max(int &a, int b) {
    a = max(a, b);
}
 
int main() {
    int n, glance_time, k;
    scanf("%d %d %d", &n, &glance_time, &k);
    int x, cnt;
    for (int i = 0; i < 2; ++i) {
        scanf("%d", &cnt);
        while (cnt--) {
            scanf("%d", &x);
            a[i][x] = 1;
        }
    }
    if (glance_time * k > 2 * n) {
        int ans = 0;
        for (int i = 1; i <= n; ++i)
            ans += (a[0][i] || a[1][i]);
        printf("%d\n", ans);
        return 0;
    }
    memset(dp, -127 / 3, sizeof dp);
    dp[0][0][0][0] = 0;
    for (int i = 1, now, pre; i <= n; ++i) {
        now = i & 1;
        pre = now ^ 1;
        for (int j = 0; j <= glance_time; ++j) for (int x = 0; x < k; ++x) for (int y = 0; y < k; ++y) {
            /// update next glance turn, only update the k's position
            if (x) get_max(dp[now][j + 1][x - 1][k - 1], dp[pre][j][x][y] + (a[0][i] || a[1][i]));
            else   get_max(dp[now][j + 1][x][k - 1], dp[pre][j][x][y] + a[1][i]);
            if (y) get_max(dp[now][j + 1][k - 1][y - 1], dp[pre][j][x][y] + (a[0][i] || a[1][i]));
            else   get_max(dp[now][j + 1][k - 1][y], dp[pre][j][x][y] + a[0][i]);
            /// update this glance turn
            if (x && y) get_max(dp[now][j][x - 1][y - 1], dp[pre][j][x][y] + (a[0][i] || a[1][i]));
            else if (x) get_max(dp[now][j][x - 1][y], dp[pre][j][x][y] + a[0][i]);
            else if (y) get_max(dp[now][j][x][y - 1], dp[pre][j][x][y] + a[1][i]);
            else        get_max(dp[now][j][0][0], dp[pre][j][x][y]);
        }
        for (int j = 0; j <= glance_time; ++j) for (int x = 0; x < k; ++x) for (int y = 0; y < k; ++y)
            dp[pre][j][x][y] = -100000;
    }
    int now = n & 1, ans = 0;
    for (int j = 0; j <= glance_time; ++j) for (int x = 0; x < k; ++x) for (int y = 0; y < k; ++y) {
        ans = max(ans, dp[now][j][x][y]);
    }
    printf("%d\n", ans);
    return 0;
}
