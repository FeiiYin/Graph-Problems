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

void show(int arr[], int len) {
    for (int i = 0; i <= len; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
struct node {
    int dif, id;
    node() {
    }
    node(int d, int i) : dif(d), id(i) {}
    bool operator < (const node& k) const {
        return dif < k.dif;
    }
};

const int mod = 998244353;
const int N = 2e5 + 5;
const int INF = 250000001;

LL st[N], ed[N], cost[N];

int main() {
    int n;
    scanf("%d", &n);
    /// positive: insert, negative: erase
    priority_queue<PLI, vector<PLI>, greater<> > pq;
    for (int i = 1; i <= n; ++i) {
        scanf("%lld %lld %lld", &st[i], &ed[i], &cost[i]);
        pq.push({st[i] + cost[i], i});
//        pq.push({st[i], i});
    }
    set<PLI> event;
    LL ans = 0;
    LL cur_time = 0;
//    cout << "work start : " << endl;
    while (!pq.empty()) {
        LL now_time = pq.top().first;
        int id = pq.top().second;
        pq.pop();
//        cout << cur_time << " " << ans << endl;
//        cout << "now pick: " << now_time << " " << id << endl;
        /// cal ans
        if (!event.empty()) {
//            cout << "pick cost: " << event.begin()->first << endl;
            LL num = (now_time - cur_time) / event.begin()->first;
            ans += num;
            cur_time += num * event.begin()->first;
        }
        if (id > 0) {
            cur_time = max(cur_time, st[id]);
            event.insert({cost[id], id});
            pq.push({ed[id], -id});
        } else {
            id = -id;
            event.erase({cost[id], id});
        }
    }
    printf("%lld\n", ans);
    return 0;
}
