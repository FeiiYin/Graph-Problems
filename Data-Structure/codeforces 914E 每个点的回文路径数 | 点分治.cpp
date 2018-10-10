/*
 * @Samaritan_infi
 */
/// 题意：一棵树，每个节点上有一个字母，问经过每个节点的回文路径有多少条。回文路径定义：将这条路径上的字母重新排列之后，能够组成回文串的路径。
/// 思路：用20位的二进制记录每个字母在路径上的出现次数的奇偶性，用cnt[]保存每个二进制状态的数量。若两条路径拼在一起要回文，
/// 则它们的状态需要满足a^b=0或a^b=(1<<n)，则a=b或者a=b^(1<<n)，则计算答案时把这两种情况分别统计即可。
/// 点分治过程中，先计算从当前节点开始的路径数，再计算经过当前节点、起点终点都在子树上的路径数。
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 2e5 + 5;
vector<int> edge[N];
char arr[N];
int siz[N], max_son[N], getroot_sum, root, root_max_son;
bool visit[N];
void getroot (int now, int fa) {
	siz[now] = 1, max_son[now] = 0;
	for (int to : edge[now]) if (! visit[to] && to != fa) {
        getroot(to, now);
        siz[now] += siz[to];
        max_son[now] = max(max_son[now], siz[to]);
	}
	max_son[now] = max(max_son[now], getroot_sum - siz[now]);
	if (max_son[now] < root_max_son) root = now, root_max_son = max_son[now];
}

LL ans[N], cnt[(1 << 20) + 5];
void dfs (int now, int fa, int val, int sta) {
    sta ^= (1 << (arr[now] - 'a'));
	cnt[sta] += val;
	for (int to : edge[now])
		if (! visit[to] && to != fa)
            dfs(to, now, val, sta);
}
/// 当前子树里每一条路径的贡献值
LL cal (int now, int fa, int sta) {
    sta ^= (1 << (arr[now] - 'a'));
    ///  a ^ b = 0 || a ^ b = (1 << n) <--> a = b || a = b ^ (1 << n)
    LL sum = cnt[sta];
    for (int i = 0; i < 20; ++i) sum += cnt[sta ^ (1 << i)];
    for (int to : edge[now]) if (!visit[to] && to != fa)
        sum += cal(to, now, sta);
    ans[now] += sum; /// 相当于从 now 出发的反向边, 否则当前节点就不能加上他的祖先经过他的节点了
    return sum;
}

void solve (int now) {
    visit[now] = true;
	dfs(now, -1, 1, 0);
	LL sum = cnt[0]; /// 计算当前节点开头的路径数
	for (int i = 0; i < 20; ++i) sum += cnt[1 << i];
	ans[now] += sum; sum = 0;
	for (int to : edge[now]) if (! visit[to]) {
        dfs(to, now, -1, 1 << (arr[now] - 'a')); /// 防止同一颗子树计数
        sum += cal(to, now, 0);
        dfs(to, now, 1, 1 << (arr[now] - 'a'));
	}
	dfs(now, -1, -1, 0);
	ans[now] += sum / 2; /// 每一条链被正反计入两次
	for (int to : edge[now]) if (! visit[to]) {
        root = -1, root_max_son = N, getroot_sum = siz[to];
        getroot(to, now);
        solve(root);
	}
}

int main() {
    freopen("in.txt", "r", stdin);
    int n; scanf("%d", &n);
    int l, r;
    for (int i = 1; i < n; ++i) {
        scanf("%d %d", &l, &r);
        edge[l].push_back(r);
        edge[r].push_back(l);
    }
    for (int i = 1; i <= n; ++i) scanf("%s", arr+1);
    root = -1, root_max_son = N, getroot_sum = n;
    getroot(1, -1);
    solve(root);
    for (int i = 1; i <= n; ++i) printf("%lld ", ans[i]);
    return 0;
}
