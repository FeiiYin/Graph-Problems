/*
 * @Samaritan_infi
 */
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;

const int N = 505;
const int M = 5005;

bool vis[N][M];
pair<pair<int, int>, int> pre[N][M];
#define mp make_pair
/// mod == x, sum == y, end with == z;
struct node {
    int x, y;
    node (int _x, int _y) : x(_x), y(_y) {}
};

void bfs (int d, int sum) {
    queue<node> que;
    vis[0][0] = true; que.push(node(0, 0));
    while (! que.empty()) {
        node tmp = que.front(); que.pop();
        int x = tmp.x, y = tmp.y;
        for (int i = 0; i <= 9; ++i) {
            int xx = (x * 10 + i) % d, yy = (y + i);
            if (yy > sum) break;
            if (! vis[xx][yy]) {
                vis[xx][yy] = true;
                pre[xx][yy] = mp(mp(x, y), i);
                que.push(node(xx, yy));
            }
        }
    }
}

int main () {
    int d, sum; scanf("%d %d", &d, &sum);
    memset(vis, 0, sizeof vis);
    bfs(d, sum);
    if (! vis[0][sum]) puts("-1");
    else {
        int x = 0, y = sum;
        vector<int> ans;
        ans.clear();
        while (true) {
            ans.push_back(pre[x][y].second);
            int xx = pre[x][y].first.first, yy = pre[x][y].first.second;
            x = xx, y = yy;
            if (! y) break;
        }
        for (int i = ans.size() - 1; i >= 0; i--)
            printf("%d", ans[i]);
        puts("");
    }
    return 0;
}
