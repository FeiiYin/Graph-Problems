/*
 * @Samaritan_infi
 */
/// 题意: 给30个点 求能覆盖所有点的最小球的半径  输入: n, (x, y, z)
/// 题解: 题解: 可以知道所谓最小球的球心，它必然处于一个稳定态，也就是与它距离最远的点最多有4个且等距离。
/// 于是，我们首先任选一个点作为球心，并找到点集中与它距离最远的点，我们让球心靠近最远的点，不断重复此过程，就可以让球心达到稳定态了！
#include <iostream>  
#include<cstdio>  
#include<algorithm>  
#include<cstring>  
#include<cmath>  
using namespace std;  
const double eps=1e-7;  
struct point3D  
{  
    double x,y,z;  
} data[35];  
int n;  
double dis(point3D a,point3D b)  
{  
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));  
}  
double solve()  
{  
    double step=100,ans=1e30,mt;  
    point3D z;  
    z.x=z.y=z.z=0;  
    int s=0;  
    while(step>eps)  
    {  
        for(int i=0; i<n; i++)  
            if(dis(z,data[s])<dis(z,data[i])) s=i;  
        mt=dis(z,data[s]);  
        ans=min(ans,mt);  
        z.x+=(data[s].x-z.x)/mt*step;  
        z.y+=(data[s].y-z.y)/mt*step;  
        z.z+=(data[s].z-z.z)/mt*step;  
        step*=0.98;  
    }  
    return ans;  
}  
int main()  
{ // freopen("t.txt","r",stdin);
    double ans;  
    while(~scanf("%d",&n),n)  
    {  
        for(int i=0; i<n; i++)  
            scanf("%lf%lf%lf",&data[i].x,&data[i].y,&data[i].z);  
        ans=solve();  
        printf("%.5f\n",ans);  
    }  
    return 0;  
}
