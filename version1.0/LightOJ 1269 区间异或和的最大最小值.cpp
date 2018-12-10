/*
题意是给你一个序列，求子序列区间异或起来的最大值和最小值 
首先可以利用类似前缀和的形式预处理一下，inpt[i]^=inpt[i-1] 
然后一段区间[l,r]的异或值就等于 inpt[r]^inpt[l-1] 
然后问题就转化成了求区间两个数，异或的最大值和最小值，接下来就是问题的关键

显然这是一个位运算的题目，通常都要按位，从高位到低位来考虑 
先求最大值，如果某个数的第 i位是 1的话，那么就尽可能地在其他数中找第 i位为 0的，这样贪心保证结果尽可能地大 
很不幸朴素去找的话，时间是 O(n^2)

于是我们把数处理成字符串存在一个 trie树里，然后把当前数与 trie树进行匹配 
匹配过程就是从高位到低位，从trie树的根向下走，每次走到当前位相反的位置，这样保证当前位尽可能地为 1（最大） 
比如当前数当前位为 1，如果能接下来 trie树上能走到 0，那么就走，否则只能走 1 
求最小值过程类似，只不过是尽量走到相同的位置，把当前位消成 0

一个数先匹配，完了以后再把数加到 trie树里。这是为了防止求最小的时候恰好能匹配到刚加进去的数，得到结果 0
*/
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
using namespace std;
typedef pair<int,int> Pii;
#define MST(a,b) memset(a,b,sizeof(a))
#define CLR(a) MST(a,0)
#define LL long long
#define ULL unsigned long long
int maxx(int a,int b){return a>b?a:b;}
int minn(int a,int b){return a<b?a:b;}
int abss(int a){return a<0?(-a):a;}

const int maxn=50000+10,noden=50000*32+10;
struct trnode
{
    int now,nxt[2];
};

int N,trien;
int inpt[maxn];
trnode trie[noden];
int find(int,int);
void insert(int);
//void bpri(int);

int main()
{
    int T;
    scanf("%d", &T);
    for(int ck=1; ck<=T; ck++)
    {
        trien=0;
        CLR(trie);
        scanf("%d", &N);
        for(int i=1; i<=N; i++)
        {
            scanf("%d", &inpt[i]);
            inpt[i]^=inpt[i-1];
        }
        insert(0);
        int sum=0,tmax=-1,tmin=(-1)^(1<<31);
        for(int i=1; i<=N; i++)
        {
            tmax=maxx(tmax, find(inpt[i],0));
            tmin=minn(tmin, find(inpt[i],1));
            insert(inpt[i]);
        }
        printf("Case %d: %d %d\n", ck, tmax, tmin);
    }
    return 0;
}

int find(int num,int xnum)
{
    int np=0;
    for(int mask=1<<30; mask; mask>>=1)
    {
        int now=(num&mask)?1:0;
        now^=xnum;
        if(trie[np].nxt[now^1]) np=trie[np].nxt[now^1];
        else if(trie[np].nxt[now]) np=trie[np].nxt[now];
        else break;
        num^=(trie[np].now)?mask:0;
    }
    return num;
}

void insert(int num)
{
    int np=0;
    for(int mask=1<<30; mask; mask>>=1)
    {
        int now=(num&mask)?1:0;
        if(trie[np].nxt[now])
        {
            np=trie[np].nxt[now];
        }
        else
        {
            trie[np].nxt[now]=++trien;
            np=trien;
            trie[np].now=now;
        }
    }
}
