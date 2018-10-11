/*
 * @Samaritan_infi
 */
/// 题意: 给你起点数，目标数，每次可以选择+1%p, -1%p, ^(p-2)%p, 要求在200步之内求出路径
/// 题解: 双向搜索+map维护路径
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1e6 + 5;
#define PII pair<int, int>
#define fi first
#define se second
int MOD; int u, v, p; /// st, ed, mod
map<int, PII> mp; /// 用来记录路径
LL q_pow(LL n, LL k) {
    LL ans = 1;
    while (k) {
        if(k&1) ans = (ans * n) % MOD;
        n = (n*n) % MOD; k >>= 1;
    }
    return ans;
}
void print(int pos1, int pos2, int ty, int tt) {
    vector<int> vc;
    if(ty == 1) {
        int t = pos1;
        while(t != u) {
            if (mp[t].fi == 1) {
                vc.push_back(1);
                t = (t + MOD - 1) % MOD;
            } else if (mp[t].fi == 2) {
                t = (t + 1) % MOD;
                vc.push_back(2);
            } else if (mp[t].fi == 3) {
                 t = q_pow(t, MOD - 2) % MOD;
                vc.push_back(3);
            }
        }
        reverse(vc.begin(), vc.end());
        vc.push_back(tt);
        t = pos2;
        while (t != v) {
            if (mp[t].fi == 1) {
                vc.push_back(2);
                t = (t + MOD - 1) % MOD;
            } else if (mp[t].fi == 2) {
                t = (t + 1) % MOD;
                vc.push_back(1);
            } else if (mp[t].fi == 3) {
                t = q_pow(t, MOD - 2) % MOD;
                vc.push_back(3);
            }
        }
    }
    else {
        int t = pos2;
        while(t != u) {
            if(mp[t].fi == 1) {
                vc.push_back(1);
                t = (t + MOD - 1) % MOD;
            } else if(mp[t].fi == 2) {
                t = (t + 1) % MOD;
                vc.push_back(2);
            } else if(mp[t].fi == 3) {
                 t = q_pow(t, MOD - 2) % MOD;
                vc.push_back(3);
            }
        }
        reverse(vc.begin(), vc.end());
        if(tt != 3) vc.push_back(3-tt);
        else vc.push_back(tt);
        t = pos1;
        while(t != v) {
            if (mp[t].fi == 1) {
                vc.push_back(2);
                t = (t + MOD - 1) % MOD;
            } else if (mp[t].fi == 2) {
                t = (t + 1) % MOD;
                vc.push_back(1);
            } else if (mp[t].fi == 3) {
                 t = q_pow(t, MOD - 2) % MOD;
                vc.push_back(3);
            }
        }
    }
    printf("%d\n", (int)vc.size());
    for (int i = 0; i < vc.size(); i++) printf("%d ", vc[i]);
}
int main() {
    scanf("%d %d %d", &u, &v, &p); MOD = p;
    if(u == v) return 0 * puts("0");
    queue<PII> q;
    q.push({u, 1}); q.push({v, 2});
    mp[u] = {u, 1}; mp[v] = {v, 2};
    while(! q.empty()) {
        PII p = q.front(); q.pop();
        int a = (p.fi + 1) % MOD;
        if(mp.find(a) != mp.end() && mp[a].se != p.se) {
            print(p.fi, a, p.se, 1);
            break;
        }
        else if(mp.find(a) == mp.end()){
            mp[a] = {1, p.se};
            q.push({a, p.se});
        }
        a = (p.fi + MOD - 1) % MOD;
        if(mp.find(a) != mp.end() && mp[a].se != p.se) {
            print(p.fi, a, p.se, 2);
            break;
        }
        else if(mp.find(a) == mp.end()){
            mp[a] = {2, p.se};
            q.push({a, p.se});
        }
        a = q_pow(p.fi, MOD - 2) % MOD;
        if(mp.find(a) != mp.end() && mp[a].se != p.se) {
            print(p.fi, a, p.se, 3);
            break;
        }
        else if(mp.find(a) == mp.end()){
            mp[a] = {3, p.se};
            q.push({a, p.se});
        }
    }
    return 0;
}
