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

LL quick_pow (LL a, LL b, LL mod) {
    LL ans = 1, base = a;
    while (b) {
        if (b & 1) (ans *= base) %= mod;
        b >>= 1;
        (base *= base) %= mod;
    }
    return ans;
}

//struct node {
//    int pos, len;
//    bool operator < (const node& k) const {
//        return pos < k.pos;
//    }
//};

const int mod = 1e9 + 7;
const int N = 2e5 + 5;
/// n个区间覆盖[1, n]的方案数
int dp[N], pre[N];
int work_dp(vector<PII>& intervals) {
    intervals.emplace_back(-100, -100);
    sort(intervals.begin(), intervals.end(), [](PII a, PII b) {
         return a.second == b.second ? a.first < b.first : a.second < b.second;
    });
//    for (auto a : intervals) {
//        cout << a.fi << " " << a.se << endl;
//    }
    LL ans = 0;
    pre[0] = 0;
    int n = intervals.size() - 1;
    /// dp[i] [1, i]区间覆盖到i.r的方案数，且使用了i的区间
    for (int i = 1, j = 1; i <= n; ++i) {
        /// 区间都是闭区间且离散的，所以不覆盖是[j.r, i.l-1), i.l-1, [i.l, i.r]
//        cout << intervals[i].first << " " << intervals[i].second << endl;
        while (intervals[j].second < intervals[i].first - 1)
            ++j;
        dp[i] = (pre[i - 1] - pre[j - 1] + mod) % mod;
        /// 自己可以覆盖
        if (intervals[i].first == 1)
            dp[i]++;
        if (intervals[i].second == n)
            ans = (ans + dp[i]) % mod;
        pre[i] = (pre[i - 1] + dp[i]) % mod;
    }
//    for (int i = 0; i <= n; ++i) cout << dp[i] << " "; cout << endl;
    return ans;
}

struct node {
    int pos, v, id;
    int l, r;
    bool operator < (const node& k) const {
        return v > k.v;
    }
};
node arr[N];
int temp[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &arr[i].pos, &arr[i].v);
        temp[i] = arr[i].v;
        arr[i].id = i;
    }
    sort(temp, temp + n);
    int temp_len = unique(temp, temp + n) - temp;
    for (int i = 0; i < n; ++i) {
        arr[i].v = lower_bound(temp, temp + temp_len, arr[i].v) - temp;
    }
    sort(arr, arr + n);
    /// get L
    vector<PII> L(n);
    for(int i = 0; i < n; ++i)
        L[i] = {arr[i].pos, arr[i].id};
    sort(L.begin(), L.end());
    int cur = n - 1;
    for(int i = 0; i < n; ++i) {
        while (cur >= 0) {
            if (arr[i].pos <= L[cur].first)
                arr[L[cur].second].r = n - i, cur--;
            else
                break;
        }
    }

    cur = 0;
    for (int i = n - 1; i >= 0; --i) {
        while (cur < n) {
            if (arr[i].pos >= L[cur].first)
                arr[L[cur].second].l = n - i, cur++;
            else
                break;
        }
    }
    vector<PII> intervals(n);

    for (int i = 0; i < n; ++i)
        intervals[i] = {arr[i].l, arr[i].r};

    int ans = work_dp(intervals);
    printf("%d\n", ans);
    return 0;
}
