/*
 * @Samaritan_infi
 */
/// 题意: n个点, m条边的无相图, 且满足图中任意一个环内都是一个完全子图。
///     q次询问，求给出两点间的最短距离, n 1e5, m 5e5
/// 条件意味着，图中任意一个点双联通分量内的点的距离都是1。这样我们就可以用tarjan算法对这个图进行缩点建图，
/// 找出图中所有的割顶，再加入新的节点变成割顶。缩完点之后的图就变成一棵树了，接下来要求两点之间的距离的话，
/// 就变成求树上两点之间的距离了，只需要借助LCA就能求解了。
/// 通过点双连通分量的性质，我们知道缩完点后的图，原来的节点之间不存在边，每个节点都会向自己所属的联通分量的割顶连边，
/// 所以在这棵树上的距离是原来距离的两倍，最后答案再除以2就行了。
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 1e5 + 7;
int n, m, q;
int low[N], dfn[N], stk[N], dsz, ssz, bsz;
int dep[N << 1], ST[N << 1][21];
vector<int>E[N], G[N << 1];

void tarjan(int u, int fa) {
	dfn[u] = low[u] = ++dsz;
	stk[++ssz] = u;
	for (auto v : E[u]) {
		if (v == fa) continue;
		if (!dfn[v]) {
			tarjan(v, u);
			low[u] = min(low[u], low[v]);
			if (low[v] >= dfn[u]) {
				++bsz;
				int now;
				do {
					now = stk[ssz--];
					G[bsz].push_back(now); G[now].push_back(bsz);
				} while (now != v);
				G[bsz].push_back(u); G[u].push_back(bsz);
			}
		} else
			low[u] = min(low[u], dfn[v]);
	}
}
void dfs(int u, int fa, int d) {
	dep[u] = d; ST[u][0] = fa;
	for (int i = 1; i <= 20; i++)
		ST[u][i] = ST[ST[u][i - 1]][i - 1];
	for (auto v : G[u]) if (v != fa)
		dfs(v, u, d + 1);
}
int LCA(int u, int v) {
	while (dep[u] != dep[v]) {
		if (dep[u] < dep[v]) swap(u, v);
		int d = dep[u] - dep[v];
		for (int i = 0; i <= 20; i++)
			if ((d >> i) & 1) u = ST[u][i];
	}
	if (u == v) return u;
	for (int i = 20; i >= 0; i--) if (ST[u][i] != ST[v][i]) {
			u = ST[u][i]; v = ST[v][i];
    }
	return ST[u][0];
}
void pre_solve() {
	bsz = n; tarjan(1, 0); dfs(1, 0, 1);
}
int main() {
	scanf("%d%d%d", &n, &m, &q);
	for (int i = 0; i < m; i++) {
		int u, v; scanf("%d%d", &u, &v);
		E[u].push_back(v); E[v].push_back(u);
	}
	pre_solve();
	while (q--) {
		int u, v; scanf("%d%d", &u, &v);
		int lca = LCA(u, v);
		int ans = dep[u] + dep[v] - 2 * dep[lca];
		printf("%d\n", ans >> 1);
	}
	return 0;
}
