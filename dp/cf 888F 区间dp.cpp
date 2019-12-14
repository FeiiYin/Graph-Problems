/// 逆时针给出n个点，这n个点形状构成正n边形
给出矩阵，表示任意两点间是否允许有边，求问有多少种连边的方法使这n个点构成一颗树，且线段两两不在除这n个点外的地方相交

显然当i和j相连，线段将剩余的点划分为线段的左右两个部分，自然联想到使用区间dp
以dir[i][j]表示将i到j间的所有点构成一棵树，且ij间有线段的方案数
以ans[i][j]表示i到j间的所有点构成一棵树，且不要求ij间有线段的方案数
#include <cstdio>
#include <cstring>
#include <algorithm>
#define MOD 1000000007
using namespace std;
typedef long long LL;

const int maxn = 5e2 + 10;
int n;
int g[maxn][maxn];
int dir[maxn][maxn], ans[maxn][maxn];


int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &g[i][j]);
        }
    }
    for (int i = 0; i < n; i++) dir[i][i] = ans[i][i] = 1;
    for (int len = 2; len <= n; len++) {
        for (int i = 0, j = (i + len - 1) % n; i < n; i++, j = (j + 1) % n) {
            for (int k = i; k != j; k = (k + 1) % n) {
                if (g[i][j]) {
                    dir[i][j] = (dir[i][j] + 1LL * ans[i][k] * ans[(k + 1) % n][j] % MOD) % MOD;
                }
                ans[i][j] = (ans[i][j] 
                    + 1LL * dir[i][(k + 1) % n] * ans[(k + 1) % n][j] % MOD) % MOD;
            }
        } 
    }
    printf("%d\n", ans[0][n - 1]);
    return 0;
}
