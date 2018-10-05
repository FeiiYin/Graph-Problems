/*
 * @Samaritan_infi
 */
/// 题目链接: http://codeforces.com/problemset/problem/713/D
/// 题意: 一个n*m的01矩阵，Q个询问，每次询问一个矩形区域内，最大的全1正方形的边长是多少？
/// 输入: n, m,  n*m 区域的0, 1
///       query_number,  x1, y1, x2, y2
/// 题解: dp[0][0][i][j]表示以(i, j)为右下角的正方形的最长边长。二维ST表维护，二分答案。
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 1e3 + 5;
int x1, y1, x2, y2, n, m;
int a[N][N];
int dp[10][10][N][N];
int Log[N];
int ask(int x1, int y1, int x2, int y2){
    int k1 = Log[x2-x1+1], k2 = Log[y2-y1+1];
    int ans = max(dp[k1][k2][x1][y1], dp[k1][k2][x1][y2-(1<<k2)+1]);
    ans = max(ans, dp[k1][k2][x2-(1<<k1)+1][y1]);
    ans = max(ans, dp[k1][k2][x2-(1<<k1)+1][y2-(1<<k2)+1]);
    return ans;
}
bool test(int l){
    //dp[0][x1][y1] dp[0][x2-x+1][y2-x+1]
    int ret = ask(x1+l-1, y1+l-1, x2, y2);
    return ret >= l;
}
void init(){
    for(int i = 1; i <= Log[n]; i++)
        for(int x = 1; x+(1<<i)-1 <= n; x++)
            for(int y = 1; y <= m; y++)
                dp[i][0][x][y] = max(dp[i-1][0][x][y], dp[i-1][0][x+(1<<i-1)][y]);
    for(int i = 0; i <= Log[n]; i++)
        for(int j = 1; j <= Log[m]; j++)
            for(int x = 1; x+(1<<i)-1 <= n; x++)
                for(int y = 1; y+(1<<j)-1 <= m; y++)
                    dp[i][j][x][y] = max(dp[i][j-1][x][y], dp[i][j-1][x][y+(1<<j-1)]);
}
int main(){
    for(int i = 2; i < N; i++) Log[i] = Log[i>>1]+1;
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++){
            scanf("%d", &a[i][j]);
            if(a[i][j] == 0) dp[0][0][i][j] = 0;
            else{
                int len = min(dp[0][0][i-1][j], dp[0][0][i][j-1]);
                dp[0][0][i][j] = len+a[i-len][j-len];
            }
        }
    init();

    int t; scanf("%d", &t);
    while(t--){
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        int L = 0, R = min(x2-x1+1, y2-y1+1);
        while(L < R){
            int M = L+R+1 >> 1;
            if( test(M) ) L = M;
            else          R = M-1;
        }
        printf("%d\n", L);
    }
    return 0;
}
