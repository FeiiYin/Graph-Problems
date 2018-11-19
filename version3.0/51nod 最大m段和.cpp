/*
 * @Samaritan_infi
 */
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 2000005;
LL s[N];
set<pair<LL, int> > a;
int n, m, now;
int pr[N], ne[N];
void Erase(int x) {
    int l = pr[x], r = ne[x];
    if (l) ne[l] = r;
    if (r) pr[r] = l;
}
int main() {
    scanf("%d %d", &n, &m);
    LL tem = 0, ans = 0;
    int cnt = 0;
    for(int i = 1, x; i <= n; ++i) {
        scanf("%d", &x);
        if((tem < 0 && x > 0) || (tem > 0 && x < 0)) {
            now += tem > 0; s[++cnt] = tem;
            a.insert(make_pair(abs(tem), cnt));
            tem = 0;
        }
        tem += x; ans += x > 0 ? x : 0;
    }
    now += tem > 0; s[++cnt] = tem;
    a.insert(make_pair(abs(tem), cnt));
    for( int i = 1; i <= cnt; ++i) pr[i] = i - 1, ne[i] = i + 1;
    ne[cnt] = s[0] = 0;
    while (now > m) {
        int x = (*a.begin()).second;
        a.erase(make_pair(abs(s[x]), x));
        if (s[x] < 0 && (!pr[x] || !ne[x]) || !s[x]) continue;
        a.erase(make_pair(abs(s[pr[x]]), pr[x])),a.erase(make_pair(abs(s[ne[x]]), ne[x]));
        ans -= abs(s[x]);
        s[x] = s[x] + s[pr[x]] + s[ne[x]];
        Erase(pr[x]), Erase(ne[x]);
        a.insert(make_pair(abs(s[x]), x));
        --now;
    }
    printf("%I64d\n", ans);
    return 0;
}
/*
2
-2 11 -4 13 -5 6 -2
2
-10 2 -9 3 -8 4
1
-1 1 -1 1 -1
*/
