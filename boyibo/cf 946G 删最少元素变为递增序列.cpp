/// 题意：给你一个序列，然后问你最少替换几个数字可以使得这个序列变成Almost Increasing Array。这个序列可以，删除其中一个数字可以变成一个严格递增的序列。
/// 首先是考虑简化问题。如果不考虑删除数字和严格，也即把一个序列替换成不下降的序列最少替换个数。这个问题很显然，用n-LIS即可。再进一步，如果要求严格递增呢？
/// 这就是一个在此之前居然不知道的经典套路：首先把每个数字减去其对应位权，然后再求n-LIS即可，即序列变成{ai-i}。道理也很容易理解。最后，再考虑一下，
/// 如果可以删除一个数字。相当于是，以被删除的数字作为分解点，之前是上一个问题，每个数字减去位权；之后是减去位权再加一。即，假设删除ak，那么在k之前的序列
/// 是{ai-i}，之后是{ai-i+1}。
///  如此一来，最终问题，还是LIS问题。我们可以考虑改一下dp，用f[]和g[]表示k之前和k之后的LIS。g可以从f转移过来，二分的优化仍然成立。
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
const int N = 1e6 + 5;
const int M = 50+ 5;
const int logN = 20;
const int INF = 0x3f3f3f3f;

int arr[N];
int f[N], g[N];
int main() {
    int n;
    memset(f, INF, sizeof f);
    memset(g, INF, sizeof g);
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", &arr[i]);
    int pos, lis = 0;
    /// 枚举每一个位置
    for (int i = 2; i <= n; ++i) {
        pos = upper_bound(g + 1, g + 1 + n, arr[i] - i + 1) - g;
        g[pos] = arr[i] - i + 1;
        lis = max(lis, pos);

        pos = upper_bound(f + 1, f + 1 + n, arr[i - 1] - i + 1) - f;
        f[pos] = arr[i - 1] - i + 1;
        g[pos] = min(f[pos], g[pos]);
        lis = max(lis, pos);
    }
    printf("%d\n", n - lis - 1);
    return 0;
}
