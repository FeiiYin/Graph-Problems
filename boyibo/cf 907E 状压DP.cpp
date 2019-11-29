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

LL quick_pow (LL a, LL b, LL mod) {
    LL ans = 1, base = a;
    while (b) {
        if (b & 1) (ans *= base) %= mod;
        b >>= 1;
        (base *= base) %= mod;
    }
    return ans;
}

const int mod = 1e9 + 7;
const int N = 2e2 + 5;
const int INF = 0x3f3f3f3f;

struct node {
    int pos, v;
    bool operator < (const node& k) const {
        return v > k.v;
    }
};


vector<int> edge[30];
bool pic[30][30];
int status[30];
bool vis[30];
const int M = 5e6 + 5;
int dp[M];
int pre[M];
int out[M];

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int l, r;
    memset(pic, 0, sizeof pic);
    memset(status, 0, sizeof status);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &l, &r);
        l--;
        r--;
        pic[l][r] = pic[r][l] = true;
        edge[l].pb(r);
        edge[r].pb(l);

        status[l] |= (1 << r);
        status[r] |= (1 << l);
    }
    if (m == n * (n - 1) / 2) {
        puts("0");
        return 0;
    }
    int tot = 1 << n;
    int ans = n;
    int ans_out = tot - 1;
    memset(dp, INF, sizeof dp);
    for (int i = 0; i < n; ++i) {
        status[i] |= (1 << i);
        dp[ status[i] ] = 1;
        pre[ status[i] ] = 0;
        out[ status[i] ] = i;
    }

    for (int s = 1; s < tot; ++s) {
        if (dp[s] == INF) continue;
        for (int i = 0; i < n; ++i) {
            int to = s | status[i];
            if (s & (1 << i) && dp[s] + 1 < dp[to]) {
                dp[to] = dp[s] + 1;
                pre[to] = s;
                out[to] = i;
            }
        }
    }
    printf("%d\n", dp[tot - 1]);
    int cur = tot - 1;
    while (pre[cur] != 0) {
        printf("%d ", out[cur] + 1);
        cur = pre[cur];
    }
    printf("%d\n", out[cur] + 1);
    return 0;
}
