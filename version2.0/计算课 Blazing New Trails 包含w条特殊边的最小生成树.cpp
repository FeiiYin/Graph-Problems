/*
 * @Samaritan_infi
 */
/// 题目链接: https://nanti.jisuanke.com/t/32253
/// 题意: 给出一个图，其中 k 个点为特殊点，特殊点和普通点的连边称为特殊边，
///       求恰好包含w条特殊边的连通图，即最小生成树
/// 题解: 由于要选恰好 w 条特殊边，所以每次给所有特殊边加上一个权重，再跑最小生成树
///       如果此时恰好包含，即可退出，通过二分权重求解，权值乘二很关键
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

const int MAXN = 1e6;  const int MAXE = 1e6;
const long long INF = 100000;

typedef pair<long long, int> PLI;
bool special_point[MAXN];
int av[MAXE], bv[MAXE];  long long cv[MAXE];
int par[MAXN];
int find_par(int n) {
   if (par[n] == n)  return n;
   return par[n] = find_par(par[n]);
}
bool unite (int x, int y) {
    int a = find_par(x), b = find_par(y);
    if (a == b) return false;
    par[a] = b;
    return true;
}
int main(int argc, char *argv[]) {
    LL n, m, k, w; scanf("%lld %lld %lld %lld", &n, &m, &k, &w);
    int v, a, b, c ;
    for (int i = 0; i < k; i++) {
        scanf("%d", &v); v-- ;
        special_point[v] = 1 ;
    }
    vector<PLI> unsp_edge, sp_edge, all;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a, &b, &c); a--; b--;
        av[i] = a; bv[i] = b;
        cv[i] = 2LL * c;  /// 不改成乘 2 会wa，但是找不到反例
        if (special_point[a] == special_point[b])
            unsp_edge.push_back(make_pair(cv[i], i));
        else
            sp_edge.push_back(make_pair(cv[i], i));
    }
    sort(unsp_edge.begin(), unsp_edge.end()) ;
    sort(sp_edge.begin(), sp_edge.end()) ;
    long long hi = 2 * (INF + 100), lo = -hi;
    long long ans = INT_MAX;
    long long hiside = 0, loside = 0;

    while (hi >= lo) {
        long long mid = (hi + lo) >> 1;
        for (int i = 0; i < sp_edge.size(); i++)
            sp_edge[i].first = cv[sp_edge[i].second] + mid ;
        all.clear();
        merge(sp_edge.begin(), sp_edge.end(), unsp_edge.begin(), unsp_edge.end(), back_inserter(all));
        for (int i = 0; i < n; i++) par[i] = i;
        long long cost = 0 ;
        int edge_num = 0, special_num = 0;
        for (int i = 0; edge_num < n-1 && i < all.size(); i++) {
            int e = all[i].second;
            if (unite(av[e], bv[e])) {
                edge_num++;
                cost += cv[e];
                if (special_point[av[e]] != special_point[bv[e]])
                    special_num++;
            }
        }
        if (edge_num < n-1) { puts("-1");  return 0; }
        if (special_num < w) {
            hiside = (cost >> 1) + (mid >> 1) * (special_num - w);
            hi = mid - 1;
        } else if (special_num > w) {
            loside = (cost >> 1) + (mid >> 1) * (special_num - w);
            lo = mid + 1;
        } else {
            hiside = loside = cost >> 1;
            break;
        }
    }
    if (hiside == 0 || loside == 0) ans = -1;
    else                            ans = hiside ;
    if (ans < -1)                   ans = -1;
    printf("%lld\n", ans);
    return 0;
}
