/// 给定n×m的格子块（最大5×20），以及每行每列的联通块个数，要你构造出一组满足条件的解，至少有一组解。
/// 先把每列的不同联通块个数的状态给搜出来，这个利用dfs就可以解决，然后按照列从左到右去枚举状态，这列放上一个状态后要记得更新每行的联通块个数。dfs即可。
/// 把a,b,c,d,e给hash成一个数字放进set中判重。
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


int st[5][20];
int n, m;
void dfs1(int state, bool have, int ge, int pos) {
    if (pos == n) {
        st[ge][0]++;
        st[ge][st[ge][0]] = state;
        return;
    }
    dfs1(state, false, ge, pos + 1);
    if (have) dfs1(state|(1 << pos), true, ge, pos + 1);
    else dfs1(state|(1 << pos), true, ge + 1, pos + 1);
}

int a[10], b[25];
char ans[6][25];
set<int> ha[20][33];
bool dfs(int pos,int pre) {
    if (pos == m) {
        for (int i = 0; i < n; i++) if (a[i] != 0) return false;
        for (int i = 0; i < n; i++) ans[i][pos] = '\0';
        return true;
    }
    int tmp = 1,pp = 0;
    for (int i = 0; i < 5; i++) {
        pp += a[i]*tmp;
        tmp *= 11;
    }
    if (ha[pos][pre].find(pp) != ha[pos][pre].end()) return false;
    for (int i = 1; i <= st[b[pos]][0]; i++) {
        int d = st[b[pos]][i];
        for (int j = 0;j < n; j++)
            if ((d&(1<<j)) && (pre & (1<<j)) == 0)
                a[j]--;
        int flag = 0;
        for (int j = 0; j < n; j++)
            if (a[j] < 0 || ((m-1-pos)/2+1 < a[j])) {
                flag = 1;
                break;
            }
        if (flag == 0 && dfs(pos+1,d)) {
            for (int j = 0; j < n; j++)
                ans[j][pos] = (d&(1<<j)) ? '*' : '.';
            return true;
        }
        for (int j = 0; j < n; j++)
            if ((d&(1<<j)) && (pre & (1<<j)) == 0)
                a[j]++;
    }
    ha[pos][pre].insert(pp);
    return false;
}
int main() {
    scanf("%d %d", &n, &m);
    dfs1(0,0,0,0);
    for (int i = 0; i < n; i++) scanf("%d",&a[i]);
    for (int i = n+1; i < 5; i++) a[i] = 0;
    for (int i = 0; i < m; i++) scanf("%d",&b[i]);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < 33; j++)
            ha[i][j].clear();
    dfs(0,0);
    for (int i = 0; i < n; i++)
        printf("%s\n",ans[i]);
    return 0;
}
