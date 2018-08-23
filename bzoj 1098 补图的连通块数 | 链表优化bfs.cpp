/*
 * @Samaritan_infi
 * 我们先把所有的节点挂链，然后再把链表上的一个节点入队，遍历其在原图上相邻的点并做上标记，
 * 那么这时没有打上标记的点在补图上和当前节点一定有边相连因而一定在同一个联通块中，
 * 所以再把这些没有打上标记的点入队，并且在链表中除去，继续这个过程，直到队列为空时这个联通块就找出来了，
 * 再取链表上还存在的点入队寻找一个新的联通块，直到删掉所有点为止，复杂度降为了O(n + m)。
 */
#include <bits/stdc++.h>
using namespace std;
const int maxn = 100010;
const int maxm = 2000010;
int n, m, edgecnt, head[maxn];
struct edge{int v, nxt; } E[maxm*2];
struct link{int pre, nxt; }L[maxn];
void del(int x){
    L[L[x].pre].nxt = L[x].nxt;
    L[L[x].nxt].pre = L[x].pre;
}
void init(){
    memset(head, -1, sizeof(head));
    edgecnt = 0;
}
void addedge(int u, int v){
    E[edgecnt].v = v, E[edgecnt].nxt = head[u], head[u] = edgecnt++;
}
bool vis1[maxn], vis2[maxn]; //vis1标记相邻点，vis2标记在队列的存在状态
int scc[maxn], scccnt;
void bfs(){
    queue <int> q;
    memset(vis1, 0, sizeof(vis1));
    memset(vis2, 0, sizeof(vis2));
    while(L[0].nxt){//还存在节点
        int now = L[0].nxt, curscc = 1;
        del(now);
        q.push(now);
        vis2[now] = 1;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(int i = head[u]; ~i; i = E[i].nxt) vis1[E[i].v] = 1;
            for(int i = L[0].nxt; i; i = L[i].nxt){
                if(!vis1[i] && !vis2[i]){
                    vis2[i] = 1;
                    q.push(i);
                    curscc++;
                    del(i);
                }
            }
            for(int i = head[u]; ~i; i = E[i].nxt) vis1[E[i].v] = 0; //一定取消相邻节点的标记
        }
        scc[++scccnt] = curscc;
    }
}
int main(){
    init();
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= m; i++){
        int u, v;
        scanf("%d%d", &u, &v);
        addedge(u, v);
        addedge(v, u);
    }
    for(int i = 1; i <= n; i++){
        L[i-1].nxt = i; L[i].pre = i - 1;
    } L[n].nxt = 0;
    bfs();
    sort(scc + 1, scc + scccnt + 1);
    printf("%d\n", scccnt);
    for(int i = 1; i < scccnt; i++) printf("%d ", scc[i]); printf("%d", scc[scccnt]);
    return 0;
}
