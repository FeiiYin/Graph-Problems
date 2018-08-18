#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<queue>
#include<vector>
#include<algorithm>
using namespace std;
#define MAX 500000
inline int read()
{
    register int x=0,t=1;
    register char ch=getchar();
    while(ch!='-'&&(ch<'0'||ch>'9'))ch=getchar();
    if(ch=='-'){t=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-48;ch=getchar();}
    return x*t;
}
int root,tot;
struct Node
{
    int ch[2];//左右儿子
    int val;//值
    int ff;//父节点
    int size;//子树大小
    int cnt;//数字的数量
}t[MAX];
inline void pushup(int u)//下放操作
{
    t[u].size=t[t[u].ch[0]].size+t[t[u].ch[1]].size+t[u].cnt;
    //当前子树的大小是左子树大小加上右子树大小当前当前节点个数 
}
inline void rotate(int x)//旋转操作
{
    int y=t[x].ff;//y是x的父节点
    int z=t[y].ff;//z是y的父节点
    int k=(t[y].ch[1]==x);//x是y的左儿子(0)还是右儿子(1)
    t[z].ch[t[z].ch[1]==y]=x;//把x旋转为z的儿子
    t[x].ff=z;//x的父亲更新为z
    t[y].ch[k]=t[x].ch[k^1];//把x的儿子给y
    t[t[x].ch[k^1]].ff=y;//更新父节点
    t[x].ch[k^1]=y;//y变为x的
    t[y].ff=x;//y的父亲更新为x
    pushup(y);pushup(x);//更新子节点数量
}
inline void splay(int x,int goal)//旋转操作，将x旋转为goal的儿子
{
    while(t[x].ff!=goal)
    {
        int y=t[x].ff;//x的父亲节点
        int z=t[y].ff;//x的祖父节点
        if(z!=goal)//如果z不是goal
            (t[y].ch[0]==x)^(t[z].ch[0]==y)?rotate(x):rotate(y);
            //如果x和y同为左儿子或者右儿子先旋转y
            //如果x和y不同为左儿子或者右儿子先旋转x
            //如果不双旋的话，旋转完成之后树的结构不会变化
        rotate(x);//再次旋转x，将x旋转到z的位置
    }
    if(goal==0)//如果目标位置是0，则是将x旋转到根节点的位置
        root=x;//更新根节点
}
inline void insert(int x)//插入x
{
    int u=root,ff=0;//当前位置u，u的父节点ff
    while(u&&t[u].val!=x)//当u存在并且没有移动到当前的值
    {
        ff=u;//向下u的儿子，父节点变为u
        u=t[u].ch[x>t[u].val];//大于当前位置则向右找，否则向左找
    }
    if(u)//存在这个值的位置
        t[u].cnt++;//增加一个数
    else//不存在这个数字，要新建一个节点来存放
    {
        u=++tot;//新节点的位置
        if(ff)//如果父节点非根
            t[ff].ch[x>t[ff].val]=u;
        t[u].ch[0]=t[u].ch[1]=0;//不存在儿子
        t[tot].ff=ff;//父节点
        t[tot].val=x;//值
        t[tot].cnt=1;//数量
        t[tot].size=1;//大小
    }
    splay(u,0);//把当前位置移到根，保证结构的平衡
}
inline void find(int x)//查找x的位置，并将其旋转到根节点
{
    int u=root;
    if(!u)return;//树空
    while(t[u].ch[x>t[u].val]&&x!=t[u].val)//当存在儿子并且当前位置的值不等于x
        u=t[u].ch[x>t[u].val];//跳转到儿子，查找x的父节点
    splay(u,0);//把当前位置旋转到根节点
}
inline int Next(int x,int f)//查找x的前驱(0)或者后继(1)
{
    find(x);
    int u=root;//根节点，此时x的父节点（存在的话）就是根节点
    if(t[u].val>x&&f)return u;//如果当前节点的值大于x并且要查找的是后继
    if(t[u].val<x&&!f)return u;//如果当前节点的值小于x并且要查找的是前驱
    u=t[u].ch[f];//查找后继的话在右儿子上找，前驱在左儿子上找
    while(t[u].ch[f^1])u=t[u].ch[f^1];//要反着跳转，否则会越来越大（越来越小）
    return u;//返回位置
}
inline void Delete(int x)//删除x
{
    int last=Next(x,0);//查找x的前驱
    int next=Next(x,1);//查找x的后继
    splay(last,0);splay(next,last);
    //将前驱旋转到根节点，后继旋转到根节点下面
    //很明显，此时后继是前驱的右儿子，x是后继的左儿子，并且x是叶子节点
    int del=t[next].ch[0];//后继的左儿子
    if(t[del].cnt>1)//如果超过一个
    {
        t[del].cnt--;//直接减少一个
        splay(del,0);//旋转
    }
    else
        t[next].ch[0]=0;//这个节点直接丢掉（不存在了）
}
inline int kth(int x)//查找排名为x的数
{
    int u=root;//当前根节点
    if(t[u].size<x)//如果当前树上没有这么多数
        return 0;//不存在
    while(1)
    {
        int y=t[u].ch[0];//左儿子
        if(x>t[y].size+t[u].cnt)
        //如果排名比左儿子的大小和当前节点的数量要大
        {
            x-=t[y].size+t[u].cnt;//数量减少
            u=t[u].ch[1];//那么当前排名的数一定在右儿子上找
        }
        else//否则的话在当前节点或者左儿子上查找
            if(t[y].size>=x)//左儿子的节点数足够
                u=y;//在左儿子上继续找
            else//否则就是在当前根节点上
                return t[u].val;
    }
}
int main()
{
    int n=read();
    insert(+2147483647);
    insert(-2147483647);
    while(n--)
    {
        int opt=read();
        if(opt==1)
        {
            int x=read();
            insert(x);
        }
        if(opt==2)
        {
            int x=read();
            Delete(x);
        }
        if(opt==3)
        {
            int x=read();
            find(x);
            printf("%d\n",t[t[root].ch[0]].size);
        }
        if(opt==4)
        {
            int x=read();
            printf("%d\n",kth(x+1));
        }
        if(opt==5)
        {
            int x=read();
            printf("%d\n",t[Next(x,0)].val);
        }
        if(opt==6)
        {
            int x=read();
            printf("%d\n",t[Next(x,1)].val);
        }
    }
    return 0;
}
