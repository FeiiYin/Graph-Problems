/*
 * @Samaritan_infi
 */
/// 题意: 给定一颗点权树，值为选点的顺序乘以点权和，选某一点必须先选他的父亲，求最小值
/// 类似题目，桂林A, 给出两个数组，要求按原串顺序组合，值为pos乘以值，使其最小
/// 题解: 按照结点的值除以结点中个数排序，不断将当前最大的结点和其父亲结点合并，通过版本修改优先队列中元素注意更新细节
//#include <bits/stdc++.h>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <queue>
using namespace std;
typedef long long LL;
const int N = 1e3 + 5;

int val[N], par[N];
vector <int> edge[N];
int version[N], num[N], del[N]; /// version 更新版本 node 合并的结点个数 del 删去的元素
struct node {
    int cost, pos, version, num;
    node () {}
    node (int _cost, int _pos, int _num, int _version = 0)
    : cost(_cost), pos(_pos), num(_num), version(_version) {}
    bool operator < (const node & k) const {
        return 1.0 * cost / num < 1.0 * k.cost / k.num;
    }
};

int main () {
    int n, root; while (~ scanf("%d %d", &n, &root)) {
        if (! (root + n)) break;
        priority_queue <node> pq;
        for (int i = 1; i <= n; ++i) {
            scanf("%d", &val[i]);
            num[i] = 1;
            if (i != root)
                pq.push(node(val[i], i, 1));
        }
        memset(version, 0, sizeof (int) * (n + 3));
        memset(del, 0, sizeof (del[0]) * (n + 3));
        for (int i = 1; i <= n; ++i) edge[i].clear();

        int l, r, ans = 0;
        for (int i = 1; i < n; ++i) {
            scanf("%d %d", &l, &r);
            edge[l].push_back(r);
            par[r] = l;
        }
        par[root] = -1;

        while (! pq.empty()) {
            node now = pq.top(); pq.pop();
            if (del[now.pos]) continue;
            if (now.version != version[now.pos]) continue;
            int fa = par[now.pos];
            ans += now.cost * num[fa];
            num[fa] += num[now.pos];
            del[now.pos] = 1;
            version[fa]++;
            //for (int son : edge[now.pos]) par[son] = fa;
            for (int i = 0; i < edge[now.pos].size(); ++i) {
                    int son = edge[now.pos][i];
                    par[son] = fa;
                    edge[fa].push_back(son);
            }
            val[fa] += now.cost;
            if (fa != root)
                pq.push(node(val[fa], fa, num[fa], version[fa]));
        }
        ans += val[root];
        printf("%d\n", ans);
    }
    return 0;
}
