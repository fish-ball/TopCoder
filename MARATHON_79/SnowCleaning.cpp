/*
STRATEGY:

1. WORKER COUNT LIMIT
    We estimates at most how many worker is the best.
    So we store $mw, to tell the maximum workers.
    When we move time to time, the maximum can be extend to: 
        (sna / t) * co = mw * sa
        i.e.: mw = max(mw, sna * co / t / sa)
        
2. WORKERS HIRE
    Each step, if we still have jobs, meaning: mw - wk.size() > 0,
    We check if there are snow that has no adjacent workers,
  then hire workers right there, until no jobs any more.

3. WORKER SNOW DISTANCE
    We define the worker snow distance to be the Manhattan distance 
  between the position of the worker cell and the snow cell.
    The distance tells the cost for the worker to clean the snow,
  if assigned.
  
4. WORKER ASSIGNMENT
    Each step, we assign workers and the snows with a minimum cost
  max matching, where cost stands for the WORKER SNOW DISTANCE.
  
5. MOVE
    Each assigned worker will move to the target, with direction under
  priority RDLU.

6. UNASSIGN WORKER ACTION
    If some worker is not assigned, he will choose a direction to move,
  such that the destination position have a maximum space with the near
  most worker.
    i.e. to make the unassigned workers disperse.

*/


//二分图最佳匹配,kuhn munkras算法,邻接阵形式,复杂度O(m*m*n)
//返回最佳匹配值,传入二分图大小m,n和邻接阵mat,表示权值
//match1,match2返回一个最佳匹配,未匹配顶点match值为-1
//一定注意m<=n,否则循环无法终止
//最小权匹配可将权值取相反数
#include <string.h>
#define inf 1000000000
#define _clr(x) memset(x,0xff,sizeof(int)*n)

char mat[100][2500];
int match1[100];
int match2[2500];

int kuhn_munkras(int m,int n){
	int s[2500],t[2500],l1[2500],l2[2500],p,q,ret=0,i,j,k;
	for (i=0;i<m;i++)
		for (l1[i]=-inf,j=0;j<n;j++)
			l1[i]=mat[i][j]>l1[i]?mat[i][j]:l1[i];
	for (i=0;i<n;l2[i++]=0);
	for (_clr(match1),_clr(match2),i=0;i<m;i++){
		for (_clr(t),s[p=q=0]=i;p<=q&&match1[i]<0;p++)
			for (k=s[p],j=0;j<n&&match1[i]<0;j++)
				if (l1[k]+l2[j]==mat[k][j]&&t[j]<0){
					s[++q]=match2[j],t[j]=k;
					if (s[q]<0)
						for (p=j;p>=0;j=p)
							match2[j]=k=t[j],p=match1[k],match1[k]=j;
				}
		if (match1[i]<0){
			for (i--,p=inf,k=0;k<=q;k++)
				for (j=0;j<n;j++)
					if (t[j]<0&&l1[s[k]]+l2[j]-mat[s[k]][j]<p)
						p=l1[s[k]]+l2[j]-mat[s[k]][j];
			for (j=0;j<n;l2[j]+=t[j]<0?0:p,j++);
			for (k=0;k<=q;l1[s[k++]]-=p);
		}
	}
	for (i=0;i<m;i++)
		ret+=mat[i][match1[i]];
	return ret;
}


#include <cstdio>
#include <cmath>
#include <cstring>
#include <ctime>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define X(pos) (pos>>16)
#define Y(pos) (pos&0xFFFF)
#define P(x,y) ((x<<16)|y)
#define dist(p1,p2) (abs(X(p1)-X(p2))+abs(Y(p1)-Y(p2)))
#define _inside(x,y) (0<=x&&x<n&&0<=y&&y<n)
#define inside(pos) _inside(X(pos),Y(pos))
#define mov(dir,pos) (pos+dd[dir])
#define rem(dir,pos) (pos-dd[dir])

const char* dc = "RDLU";
const int dd[] = {65536, 1, -65536, -1};
char buf[20];

class SnowCleaning {
    
    // boardSize
    int n;
    // time
    int t;
    // total snow fall accumulated
    int sna;
    // max worker estimation
    int mw;
    // salary
    int sa;
    // snowFine cost
    int co;
    
    // workers
    int w[50][50];
    vector<int> wk;
    // snow
    bool s[50][50];
    vector<int> sn;
    
public:
    int init(int boardSize, int salary, int snowFine) {
        // Initialize the parameters.
        n = boardSize;
        t = 0;
        sna = 0;
        sa = salary;
        co = snowFine;
        mw = 10;
        memset(w, -1, sizeof(w));
        wk.resize(0);
        memset(s, 0, sizeof(s));
        sn.resize(0);
        
        return 0;
    }
    vector<string> nextDay(vector<int> snowFalls) {
        // result pool.
        vector<string> ans(0);
        
        // new snow fall count.
        int k2 = snowFalls.size(), k = (k2>>1);
        
        // estimates the max workers.
        sna += k;
        t += 1;
        mw = max(mw, sna * co / t / sa);
        mw = min(mw, 100);
        
        // snow falls.
        for(int i = 0, x, y; i < k2; i += 2) {
            x = snowFalls[i];
            y = snowFalls[i+1];
            if(!s[x][y]) {
                s[x][y] = true;
                sn.push_back(P(x,y));
            }
        }
        
        // hire workers
        if(mw - wk.size() > 0) {
            bool _w[100] = {};
            for(int i = 0, p, x, y; i < sn.size() && mw - wk.size() > 0; ++i) {
                p = sn[i];
                x = X(p);
                y = Y(p);
                if(w[x][y] == -1) {
                    w[x][y] = wk.size();
                    wk.push_back(p);
                    sprintf(buf, "H %d %d", x, y);
                    ans.push_back(buf);
                }
            }
        }
        
        // assignment
        memset(mat, 0, sizeof(mat));
        for(int i = 0; i < wk.size(); ++i) {
            for(int j = 0; j < sn.size(); ++j) {
                mat[i][j] = -dist(wk[i], sn[j]);
            }
        }
        
        kuhn_munkras(wk.size(), sn.size());
        
        for(int i = 0; i < wk.size(); ++i) cout << match1[i] << ' '; cout << endl;
        for(int j = 0; j < sn.size(); ++j) cout << match2[j] << ' '; cout << endl;
        
        
        
        return ans;
    }
};

int main() {
    SnowCleaning sc;
    
    int n, s, c;
    cin >> n >> s >> c;
    
    sc.init(n, s, c);
    
    for(int i = 2000; i--;) {
        int cnt;
        cin >> cnt;
        vector<int> v(cnt*2);
        for(int j = 0; j < cnt*2; j++) {
            cin >> v[j];
        }
        vector<string> ans = sc.nextDay(v);
        cout << ans.size() << endl;
        for(int j = 0; j < ans.size(); ++j) {
            cout << ans[j] << endl;
        }
    }
    
    return 0;
}
