#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define N 200010
using namespace std;
int n;
int fa[N];
int ch[N][2];
int rt[N];
int size[N];
int rev[N];
void init()
{
    for(int i=1;i<=n;i++)rt[i]=1,size[i]=1;
}
void pushup(int x)
{
    size[x]=size[ch[x][0]]+size[ch[x][1]]+1;
}
void rotate(int x)
{
    int y=fa[x],kind=ch[y][1]==x;
    ch[y][kind]=ch[x][!kind];
    fa[ch[x][!kind]]=y;
    ch[x][!kind]=y;
    fa[x]=fa[y],fa[y]=x;
    if(rt[y])
    {
        rt[y]=0,rt[x]=1;
    }else ch[fa[x]][ch[fa[x]][1]==y]=x;
    pushup(y);
}
void splay(int x)
{
    while(!rt[x])
    {
        int y=fa[x],z=fa[y];
        if(rt[y]) rotate(x);
        else
        {
            if((ch[y][1]==x)==(ch[z][1]==y))
            {
                rotate(y),rotate(x);
            }else rotate(x),rotate(x);
        }
    }
    pushup(x);
}
void access(int x)
{
    int y=0;
    while(x)
    {
        splay(x);
        rt[ch[x][1]]=1,rt[y]=0;
        ch[x][1]=y;
        pushup(x);
        y=x,x=fa[x];
    }
}
void mt(int x)
{
    access(x);
    splay(x);
}
void link(int x,int y)
{
    mt(x);
    fa[ch[x][0]]=0;
    rt[ch[x][0]]=1;
    ch[x][0]=0;
    fa[x]=y;
    pushup(x);
}
int main()
{
    scanf("%d",&n);
    init();
    for(int i=1;i<=n;i++)
    {
        int x;
        scanf("%d",&x);
        if(i+x<=n)link(i,i+x);
    }
    int m;
    scanf("%d",&m);
    while(m--)
    {
        int jd;
        scanf("%d",&jd);
        switch(jd)
        {
            int x,y;
            case 1:scanf("%d",&x);x++;mt(x);printf("%d\n",size[ch[x][0]]+1);break;
            case 2:scanf("%d%d",&x,&y);x++;if(x+y>n)link(x,0);else link(x,x+y);break;
        }
    } 
}
