#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstring>
using namespace std;

//求网络最小费用最大流,邻接阵形式
//返回最大流量,flow返回每条边的流量,netcost返回总费用
//传入网络节点数n,容量mat,单位费用cost,源点source,汇点sink

#define MAXN 110
#define inf 1000000000

int min_cost_max_flow(int n,int mat[][MAXN],int cost[][MAXN],int source,int sink,int flow[][MAXN],int& netcost){
	int pre[MAXN],min[MAXN],d[MAXN],i,j,t,tag;
	if (source==sink) return inf;
	for (i=0;i<n;i++)
		for (j=0;j<n;flow[i][j++]=0);
	for (netcost=0;;){
		for (i=0;i<n;i++)
			pre[i]=0,min[i]=inf;
		//通过bellman_ford寻找最短路，即最小费用可改进路
		for (pre[source]=source+1,min[source]=0,d[source]=inf,tag=1;tag;)
			for (tag=t=0;t<n;t++)
				if (d[t])
					for (i=0;i<n;i++)
						if (j=mat[t][i]-flow[t][i]&&min[t]+cost[t][i]<min[i])
							tag=1,min[i]=min[t]+cost[t][i],pre[i]=t+1,d[i]=d[t]<j?d[t]:j;
						else if (j=flow[i][t]&&min[t]<inf&&min[t]-cost[i][t]<min[i])
							tag=1,min[i]=min[t]-cost[i][t],pre[i]=-t-1,d[i]=d[t]<j?d[t]:j;
		if (!pre[sink])	break;
		for (netcost+=min[sink]*d[i=sink];i!=source;)
			if (pre[i]>0)
				flow[pre[i]-1][i]+=d[sink],i=pre[i]-1;
			else
				flow[i][-pre[i]-1]-=d[sink],i=-pre[i]-1;
	}
	for (j=i=0;i<n;j+=flow[source][i++]);
	return j;
}

class SpecialCells {
	map<int, int> m1, m2, s1, s2;
	int mat[MAXN][MAXN];
	int cost[MAXN][MAXN];
	int flow[MAXN][MAXN];
public:
	int getx(int i) {
		return 2 + i;
	}
	int gety(int i) {
		return 2+m1.size()+i;
	}
	int guess(vector <int> x, vector <int> y) {
		m1.clear();
		m2.clear();
		for(int i = 0; i < x.size(); ++i) {
            // 就是下面这两句害了我：后面 m1.size() 是不行的，就差个 -1。 
			if(m1.find(x[i]) == m1.end()) m1[x[i]] = m1.size()-1;
			if(m2.find(y[i]) == m2.end()) m2[y[i]] = m2.size()-1;
		}
		int netcost = 0;
		memset(mat, 0, sizeof(mat));
		memset(cost, 0, sizeof(cost));
		for(int i = 0; i < m1.size(); ++i) {
			for(int j = 0; j < m2.size(); ++j) {
				mat[getx(i)][gety(j)] = 1;
			}
		}
		for(int i = 0; i < x.size(); ++i) {
			mat[0][getx(m1[x[i]])] += 1;
			mat[gety(m2[y[i]])][1] += 1;
			cost[getx(m1[x[i]])][gety(m2[y[i]])] = 1;
		}
		int sz = 2+m1.size()+m2.size();
		return netcost;
	}
};

int main() {
    int xx[] = {1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
    int yy[] = {1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3};
    vector<int> x(xx,xx+sizeof(xx)/sizeof(int)), y(yy,yy+sizeof(yy)/sizeof(int));
    SpecialCells obj;
    cout << obj.guess(x, y) << endl;
    system("pause");
}
