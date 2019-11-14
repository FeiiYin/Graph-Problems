/**
 * @Samaritan
 */
#pragma optimization_level 3
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math,O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
 
#define fast cin.tie(0);cout.tie(0);cin.sync_with_stdio(0);cout.sync_with_stdio(0);
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count()); //uid<int> u1(5, 10); u1(rnd);
// shuffle is key
 
int main() {
    fast;
    int a; scanf("%d", &a);
    unordered_set<int> s;
    for(int q=0; q<a; q++){
        unsigned int x;
        scanf("%u", &x);
        s.insert(x);
    }
    a = s.size();
    unsigned int m[a];
    unsigned int cnt = 0;
    for(auto &i : s) m[cnt++] = i;
    shuffle(m, m+a, rnd);
    unsigned int g = (1<<30);
    for(unsigned int c = 0; c<g; c++){
        unsigned int ans = __builtin_popcount(c^m[0]);
        for(auto &i : m){
            if(__builtin_popcount(c^i)!=ans){
                ans = -1;
                break;
            }
        }
        if(ans!=-1){
            cout<<c;
            return 0;
        }
    }
    cout<<-1;
    return 0;
}
