/*
 * @Samaritan_infi
 */
/// 
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

inline int read()
{
    int x=0,t=1;char ch=getchar();
    while((ch<'0'||ch>'9')&&ch!='-')ch=getchar();
    if(ch=='-')t=-1,ch=getchar();
    while(ch<='9'&&ch>='0')x=x*10+ch-48,ch=getchar();
    return x*t;
}
const int maxn = 1e5 + 5;

int trie[maxn][26] = {0}, sum[maxn][2] = {0}, trie_num;
void trie_insert(char word[]) {
    int now = 0;
    int len = strlen(word);
    for(int i = 0; word[i]; i ++) {
        int k = word[i] - 'a';
        if(! trie[now][k]) trie[now][k] = ++ trie_num;
        now = trie[now][k];
    }
    sum[now][0] ++; /// 个数
    sum[now][1] = 1;/// 是否结尾
}

void dfs(int now) {
    for(int i = 0; i < 26; i ++) {
        if(trie[now][i]) {
            dfs(trie[now][i]);
            sum[now][0] += sum[ trie[now][i] ][0];
        }
    }
}

char str[maxn];

int ans = 0, num;
int bel[maxn];
priority_queue<int, vector<int>, less<int> > q[maxn];

int unite(int a, int b) {
    if(q[a].size() < q[b].size()) {
        while(! q[a].empty()) {
            q[b].push(q[a].top());
            q[a].pop();
        }
        return b;
    } else {
        while(! q[b].empty()) {
            q[a].push(q[b].top());
            q[b].pop();
        }
        return a;
    }
}

void DFS(int now, int val) {
    int flag = false;
    for(int i = 0; i < 26; i ++) {
        if(trie[now][i]) flag = true, DFS(trie[now][i], val + 1);
    }
    if(! flag) {
        bel[now] = ++ num;
        q[num].push(val);
        return;
    }
    for(int i = 0; i < 26; i ++) {
        if(trie[now][i]) {
            if(bel[now]) {bel[now] = unite(bel[now], bel[ trie[now][i] ]); }
            else bel[now] = bel[ trie[now][i] ];
        }
    }
    if(sum[now][1]) {
        q[ bel[now] ].push(val);
    } else {
        q[ bel[now] ].pop();
        q[ bel[now] ].push(val); /// ???
    }
}

int main() {
    trie_num = 0;
    int root = 0;
    int n = read();
    for(int i = 0; i < n; i ++) {
        scanf("%s", str);
        trie_insert(str);
    }
    dfs(root);
//    for(int i = 0; i <= 16; i ++) {
//        for(int j = 0; j < 26; j ++) {
//            cout << trie[i][j] << " ";
//        }cout << endl;
//    }
//    cout << endl;
//    for(int i = 0; i <= 16; i ++) {
//        cout << sum[i][0] << " " << sum[i][1] << endl;
//    }
    num = 0;
    for(int i = 0; i < 26; i ++) {
        if(! trie[root][i]) continue;
        DFS(trie[root][i], 1);
        int h = bel[ trie[root][i] ];
        while(! q[h].empty()) {
            ans += q[h].top(); q[h].pop();
        }
    }
    printf("%d\n", ans);
    return 0;
}
