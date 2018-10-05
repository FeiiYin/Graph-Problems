/*
 * @Samaritan_infi
 */
/// 题目链接: https://nanti.jisuanke.com/t/32255
/// 题意: 一颗有根树，给出每个点的权值和父亲，求从树中选出若干个点，能满足堆性质的最大点数
///       不需要形成一颗子树，只要祖先比孩子严格大即可
/// 输入: n  点数
///       val, par  保证 par 比自己标号小
/// 题解: 启发式合并
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define all(c) c.begin(),c.end()
const int N = 200010;
int par[N], val[N], id[N];
multiset<int> s[N];
int main() {
	int n; scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d %d", &val[i], &par[i]); id[i] = i;
	}
	for (int i = n; i >= 1; i--) {
		auto it = s[id[i]].upper_bound(val[i] - 1);
		if (it != s[id[i]].end()) s[id[i]].erase(it);
		s[id[i]].insert(val[i]);
		if (s[id[i]].size() < s[id[par[i]]].size()) {
			s[id[par[i]]].insert(all(s[id[i]]));
		} else {
			s[id[i]].insert(all(s[id[par[i]]]));
			id[par[i]] = id[i];
		}
	}
	printf("%d\n", (int)s[id[0]].size());
	return 0;
}
