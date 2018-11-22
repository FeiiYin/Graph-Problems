我们把k个反转过来，然后周围一定有一个和这个k个不相同的，然后就变成了k+1个相同的，
然后我们可以把这个两个互换位置，所以，问题就是一个祖玛游戏，每次消除连续的k个，然后看剩下的是否相同即可
#include<bits/stdc++.h>
#define xx first
#define yy second
#define mp make_pair
#define pb push_back
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int MAXN=1e6+5;
int n,k;
char s[MAXN],t[MAXN];
int st[MAXN],sz[MAXN];
string doit(char x[])
{
	if(k==1) return "";
	int len=0;
	for(int i=1;i<=n;i++)
	{
		if(len&&st[len]==x[i])
		{
			sz[len]++;
			if(sz[len]==k)
				len--;
		}
		else
		{
			len++;
			st[len]=x[i];
			sz[len]=1;
		}
	}
	string ret="";
	for(int i=1;i<=len;i++)
	{
		for(int j=1;j<=sz[i];j++)
			ret+=st[i];
	}
	return ret;
}
int main()
{
	//freopen("in.txt","r",stdin);
	//freopen("out.txt","w",stdout);
	scanf("%d%d",&n,&k);
	scanf("%s",s+1);
	scanf("%s",t+1);
	string A=doit(s);
	string B=doit(t);
	if(A==B) puts("Yes");
	else puts("No");
	return 0;
}
