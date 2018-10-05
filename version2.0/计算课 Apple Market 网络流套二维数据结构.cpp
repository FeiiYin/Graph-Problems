/*
 * @Samaritan_infi
 */
/// 题目链接: https://nanti.jisuanke.com/t/32255
/// 题意: 一个矩形区域 n * m，每个点包括一定数量的苹果，现在k个人来购买，每个人会访问一个子矩形区域，能买a个
/// 输入: n, m, k 
///       矩形区域 n * m 每个点数量
///       k个人: top, bottom, left, right, a
/// 题解: 最大流，通过二维数据结构进行优化即可
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <class Flow, size_t MAXV, size_t MAXE> struct MaxFlow {
  Flow cap[MAXE << 1], flow[MAXE << 1], limit[MAXV];
  int last[MAXV], to[MAXE << 1], next[MAXE << 1];
  int prev[MAXV], cur[MAXV], height[MAXV], cnt[MAXV + 1];
  int edgecnt, vertices;
  void resetflow() {
    fill(flow, flow + edgecnt, 0);
  }
  void init() {
    memset(last, -1, sizeof(last));
    vertices = edgecnt = 0;
  }
  void _addedge(int a, int b, Flow c) {
    flow[edgecnt] = 0;
    to[edgecnt] = b;
    cap[edgecnt] = c;
    next[edgecnt] = last[a];
    if (last[a] == -1) vertices++;
    last[a] = edgecnt++;
    assert(vertices <= (int) MAXV);
    assert(edgecnt <= (int) 2 * MAXE);
  }
  void add_edge(int start, int end, Flow capacity, bool directed = true) {
    _addedge(start, end, capacity);
    _addedge(end, start, directed ? 0 : capacity);
  }
  ll go(int source, int sink) {
    const Flow INF = numeric_limits<Flow>::max();
    ll maxflow = 0;
    memset(prev, -1, sizeof(prev));
    memset(cur, -1, sizeof(cur));
    memset(height, 0, sizeof(height));
    memset(cnt, 0, sizeof(cnt));
    fill(limit, limit + MAXV, 0);
    cnt[0] = vertices;
    int at = source, e = -1;
    limit[at] = INF;
    while (height[source] < vertices) {
      for (e = cur[at]; e > -1; e = next[e])
        if (flow[e] < cap[e] && height[to[e]] + 1 == height[at]) break;
      if (e > -1) {
        cur[at] = prev[to[e]] = e;
        limit[to[e]] = min(limit[at], cap[e] - flow[e]);
        at = to[e];
        if (at == sink)
          for (maxflow += limit[sink]; at != source; at = to[prev[at] ^ 1])
            flow[prev[at]] += limit[sink], flow[prev[at] ^ 1] -= limit[sink];
      } else {
        if (--cnt[height[at]] == 0) break;
        height[at] = vertices;
        for (e = last[at]; e > -1; e = next[e])
          if (flow[e] < cap[e] && height[to[e]] + 1 < height[at])
            height[at] = height[to[e]] + 1, cur[at] = e;
        cnt[height[at]]++;
        if (at != source) at = to[prev[at] ^ 1];
      }
    }
    return maxflow;
  }
};
const int MAX_K = 100000; const int MAX_NM = 50;
const int LOG = 6;
const int PW = MAX_NM;
MaxFlow<int, 4 * PW * PW + MAX_K + 2, MAX_NM * MAX_NM * (1 + (LOG + 1) * (LOG + 1)) + MAX_K * (1 + 4 * (LOG + 2))> net;
int cost[MAX_NM][MAX_NM];
int tree_id[2 * PW][2 * PW];
int next_node() {
  static int next_num = 0;
  return next_num++;
}
void update(const int vert, int x, int c1, int c2, int v) {
  for (int l = c1 + PW, r = c2 + PW + 1; l < r; l >>= 1, r >>= 1) {
    if (l & 1)
      net.add_edge(vert, tree_id[x][l++], v);
    if (r & 1)
      net.add_edge(vert, tree_id[x][--r], v);
  }
}
void solve() {
  int N, M, K;
  scanf("%d %d %d", &N, &M, &K);
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < M; ++j)
      scanf("%d", &cost[i][j]);
  const int src = next_node(), sink = next_node();
  net.init();
  for (int i = 1; i < 2 * PW; ++i)
    for (int j = 1; j < 2 * PW; ++j)
      tree_id[i][j] = next_node();
  for (int x = PW; x < PW + N; ++x)
    for (int y = PW; y < PW + M; ++y) {
      net.add_edge(tree_id[x][y], sink, cost[x - PW][y - PW]);
      for (int i = x; i > 0; i >>= 1)
        for (int j = y; j > 0; j >>= 1)
          if (tie(i, j) != tie(x, y))
            net.add_edge(tree_id[i][j], tree_id[x][y], cost[x - PW][y - PW]);
    }
  for (int k = 0, r1, r2, c1, c2, v; k < K; ++k) {
    scanf("%d %d %d %d %d", &r1, &r2, &c1, &c2, &v);
    --r1, --r2, --c1, --c2;
    const int vert = next_node();
    net.add_edge(src, vert, v);
    for (int l = r1 + PW, r = r2 + PW + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        update(vert, l++, c1, c2, v);
      if (r & 1)
        update(vert, --r, c1, c2, v);
    }
  }
  const ll res = net.go(src, sink);
  printf("%lld\n", res);
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  solve();
  return 0;
}
