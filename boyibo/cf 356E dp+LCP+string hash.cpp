/// gray code: 两边相等且mid char只出现一次，求最多改变一个字符的最大值
/// 通过条件 dp
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

inline int log2(const float &x) {
    return ((unsigned &) x >> 23 & 255) - 127;
}

void show(int arr[], int len) {
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

const int mod = 998244353;
const int N = 1e5 + 5;
const int logN = 30;
const int INF = 0x3f3f3f3f;

char arr[N];
int char_num[N][26];
const int hash_key = 211;
unsigned long long hash_val[N], mi[N];
int length[logN], length_cnt;
int tot_len;
/// use hash value to check whether string are same
inline bool check_string_same(int l1, int r1, int l2, int r2) {
    if (r1 - l1 != r2 - l2) return false;
    if (r1 > tot_len || r2 > tot_len) return false;
    int len = r1 - l1 + 1;
    unsigned long long s1 = hash_val[r1] - hash_val[l1 - 1] * mi[len];
    unsigned long long s2 = hash_val[r2] - hash_val[l2 - 1] * mi[len];
    return s1 == s2;
}
bool grey[N][logN];
void check_grey(int l, int length_id) {
    if (length_id == 1) {
        grey[l][length_id] = true;
        return;
    }
    int r = l + length[length_id] - 1;
    int mid = l + length[length_id - 1];
    int mid_char = arr[mid];
    if (char_num[r][mid_char] - char_num[l - 1][mid_char] == 1 &&
        check_string_same(l, mid - 1, mid + 1, r) &&
        grey[l][length_id - 1] &&
        grey[mid + 1][length_id - 1]) {
        grey[l][length_id] = true;
    }
}
int max_log;
/// check from l1, and l2 longest common prefix string's length
int LCP(int l1, int l2) {
    int len = 0;
    for (int i = max_log; i >= 0; --i) {
        int now_len = 1 << i;
        if (check_string_same(l1 + len, l1 + len + now_len - 1, l2 + len, l2 + len + now_len - 1))
            len += (1 << i);
    }
    return len;
}

LL commit[N][26];
void cal_commit(int l, int length_id) {
    if (length_id == 1) {
        for (int i = 0; i < 26; ++i) {
            if (arr[l] != i)
                commit[l][i]++;
        }
        return;
    }
    int r = l + length[length_id] - 1;
    int mid = l + length[length_id - 1];
    int mid_char = arr[mid];
    /// check mid value change commitment
    if (// char_num[r][mid_char] - char_num[l - 1][mid_char] == 1 &&
        check_string_same(l, mid - 1, mid + 1, r) &&
        grey[l][length_id - 1] &&
        grey[mid + 1][length_id - 1]) {
        for (int i = 0; i < 26; ++i) {
            if (// mid_char != i &&
                char_num[r][i] - char_num[l - 1][i] == 0) {
                commit[mid][i] += 1LL * length[length_id] * length[length_id];
            }
        }
    }
    /// check other side value change commitment
    /// should be (l, lcp, dif_char, lcp) mid (mid + 1, lcp, dif_char, lcp, r)
    /// and keep other side is grey string
    int lcp_len = LCP(l, mid + 1);
    if (lcp_len >= length[length_id - 1]) return;
    int next_lcp_len = LCP(l + lcp_len + 1, mid + 1 + lcp_len + 1);
    if (lcp_len + 1 + next_lcp_len < length[length_id - 1]) return;
    /// cannot directly check l to r, since the dif char may be the same with the mid char
    if (grey[l][length_id - 1] &&
        char_num[mid - 1][mid_char] - char_num[l - 1][mid_char] == 0) {
        commit[mid + lcp_len + 1][arr[l + lcp_len]] += 1LL * length[length_id] * length[length_id];
    }
    if (grey[mid + 1][length_id - 1] &&
        char_num[r][mid_char] - char_num[mid][mid_char] == 0) {
        commit[l + lcp_len][arr[mid + lcp_len + 1]] += 1LL * length[length_id] * length[length_id];
    }
}

LL pre_cost[N];

int main() {
    scanf("%s", arr + 1);
    int len = strlen(arr + 1);
    tot_len = len;
    /// init hash
    mi[0] = 1;
    for (int i = 1; i <= len; ++i) {
        arr[i] = arr[i] - 'a';
        char_num[i][arr[i]]++;
        for (int j = 0; j < 26; ++j) {
            char_num[i][j] += char_num[i - 1][j];
        }
        hash_val[i] = hash_val[i - 1] * hash_key + arr[i];
        mi[i] = mi[i - 1] * hash_key;
    }
    for (int i = 1; i < 30; ++i) {
        length[i] = length[i - 1] * 2 + 1;
        if (length[i] > len) {
            length_cnt = i;
            max_log = length_cnt;
            break;
        }
    }
    LL ans = 0;
    for (int length_id = 1; length_id < length_cnt; ++length_id) {
        for (int i = 1; i <= len; ++i) {
            if (i + length[length_id] - 1 > len) break;
            check_grey(i, length_id);
            if (grey[i][length_id]) {
                LL now = 1LL * length[length_id] * length[length_id];
                ans += now;
                pre_cost[i] += now;
                pre_cost[i + length[length_id]] -= now;
            }
        }
    }
    pre_cost[0] = 0;
    for (int i = 1; i <= len; ++i)
        pre_cost[i] += pre_cost[i - 1];
    for (int i = 1; i < length_cnt; ++i) {
        for (int j = 1; j <= len; ++j) {
            if (j + length[i] - 1 > len) break;
            cal_commit(j, i);
        }
    }
    LL sum = 0;
    for (int i = 1; i <= len; ++i) {
        for (int j = 0; j < 26; ++j) {
            sum = max(sum, -pre_cost[i] + commit[i][j]);
        }
    }
//    cout << ans << endl;
    printf("%I64d\n", ans + sum);
    return 0;
}
