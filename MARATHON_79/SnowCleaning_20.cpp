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
#define MAXN 2600
#define inf 1000000000
#define _clr(x) memset(x,0xff,sizeof(int)*n)

int kuhn_munkras(int m,int n,int mat[][MAXN],int* match1,int* match2){
    int s[MAXN],t[MAXN],l1[MAXN],l2[MAXN],p,q,ret=0,i,j,k;
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
#include <deque>
#include <algorithm>
#include <fstream>
using namespace std;



const char* dc = "RDLU";
const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};
char sbuf[20];

    int dk[] = {1, 0, 3, 2};


class SnowCleaning {
    
    
int mat[100][MAXN];
int match1[100];
int match2[MAXN];

int dir[100][4];
    

    
    // boardSize
    int n;
    // time
    int t;
    int sun;
    // total snow fall accumulated
    double sna;
    // max worker estimation
    int mw;
    // salary
    double sa;
    // snowFine cost
    double co;
    
    
    struct Pos {
        Pos() : x(0), y(0) {};
        Pos(int x, int y) : x(x), y(y) {};
        short x;
        short y;
        
        Pos move(int d) {
            return Pos(x+dx[d], y+dy[d]);
        }
        
        bool inside(int n) {
            return 0<=x && x<n && 0<=y && y<n;
        }
    };


    int dist(Pos a, Pos b) {
        return abs(a.x-b.x)+abs(a.y-b.y);
    }
    
    // workers
    int w[50][50];
    vector<Pos> wk;
    // snow
    int s[50][50];
    vector<Pos> sn;
    
    // worker processing queue
    deque<int> q;
    
    // result pool
    vector<string> ans;
    
    
    
    // snow falls.
    bool fall(int x, int y) {
        if(s[x][y] == -1) {
            s[x][y] = sn.size();
            sn.push_back(Pos(x,y));
            return true;
        }
        return false;
    }
    
    // hire a worker.
    bool hire(int x, int y) {
        if(w[x][y] == -1) {
            w[x][y] = wk.size();
            wk.push_back(Pos(x, y));
            // log the operation
            sprintf(sbuf, "H %d %d", x, y);
            ans.push_back(sbuf);
            return true;
        }
        return false;
    }
    
    // clean a snow.
    bool clean(int x, int y) {
        if(s[x][y] != -1) {
            int i = s[x][y];
            swap(s[x][y], s[sn.back().x][sn.back().y]);
            swap(sn[i], sn.back());
            s[x][y] = -1;
            sn.pop_back();
            return true;
        }
        return false;
    }
    
    // let worker k move towards direction d.
    inline bool walk(int k, int d) {
//printf("walk: %d %c", k, dc[d]);
        Pos p0 = wk[k], p = p0.move(d);
        if(p.inside(n) && w[p.x][p.y] == -1) {
            wk[k] = p;
            w[p.x][p.y] = k;
            w[p0.x][p0.y] = -1;
            // log the operation
            sprintf(sbuf, "M %d %c", k, dc[d]);
            ans.push_back(sbuf);
            return true;
        }
        return false;
    }
    
public:
    int init(int boardSize, int salary, int snowFine) {
        // Initialize the parameters.
        n = boardSize;
        t = 0;
        sun = 0;
        sna = 0;
        sa = salary;
        co = snowFine;
        mw = int(n*sqrt(6.0*co/sa)/4.0);
        memset(w, -1, sizeof(w));
        wk.clear();
        memset(s, -1, sizeof(s));
        sn.clear();
        return 0;
    }
    vector<string> nextDay(vector<int> snowFalls) {

        // result pool.
        ans.clear();

        // new snow fall count.
        int k2 = snowFalls.size(), k = (k2>>1);

        // estimates the max workers.
        sna += k;
        t += 1;
        sun += k2 == 0 ? 1 : 0;
        
        /*
        if(sna > 0) {
            mw = max(mw, sna * co / ((t - sun) * sa));
            mw = min(mw, 40);
        }
        */
//*/        if(mw < int(ceil(sqrt(double(sna) / t * double(co) / double(sa)) * n / 4.0))) ++mw;
mw = int(sqrt(sna * (0.5 + 0.45 * min(t,1000) / 1000.0) / t * co / sa) * n / 4.0);
/*/
        int xmw = int(sqrt(sna * (0.5 + 0.5 * t / 2000.0) / t * co / sa) * n / 4.0);
        if(mw < xmw) {
            mw += (xmw-mw) / 2;
        }
        mw = min(mw, xmw);
//*/
        //mw=20;
        mw = min(mw, 100);

        // snow falls.
        for(int i = 0; i < k2; i += 2) {
            fall(snowFalls[i], snowFalls[i+1]);
        }
        
//        mw = max(int(sn.size()), mw) / 2;
//        mw = min(mw, 100);
        
/*
for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
        cout << (w[i][j]==-1?(s[i][j] == -1 ? '.' : '#'):'X');
    }
    cout << endl;
}
cout << "sn: ";
        for(int i = 0; i < sn.size(); ++i) {
            printf("(%d, %d) ", sn[i].x, sn[i].y);
        }cout << endl;
cout << "wk: ";
        for(int i = 0; i < wk.size(); ++i) {
            printf("(%d, %d) ", wk[i].x, wk[i].y);
        }cout << endl;
//*/
        // assignment
        memset(mat, 0, sizeof(mat));
        int bound = n / 2;
        for(int i = 0; i < wk.size(); ++i) {
            for(int j = 0; j < sn.size(); ++j) {
                int dst = dist(wk[i], sn[j]);
                //mat[i][j] = - dst*dst;
                mat[i][j] = -(dst<<16)+abs(sn[j].x*2-n)+abs(sn[j].y*2-n);
                
                //mat[i][j] = - dst;
            }
            for(int j = sn.size(); j < sn.size() + wk.size(); ++j) {
                mat[i][j] = -(bound<<16);
            }
        }
        kuhn_munkras(wk.size(), sn.size()+wk.size(), mat, match1, match2);
        
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] > sn.size()) {
                match2[match1[i]] = -1;
                match1[i] = -1;
            }
        }
        
        
//cout << "match1: "; for(int i = 0; i < wk.size(); ++i) cout << match1[i] << ' '; cout << endl;
//cout << "match2: "; for(int j = 0; j < sn.size(); ++j) cout << match2[j] << ' '; cout << endl;
        
        // move assigned workers.
        q.clear();
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] > -1) {
                q.push_back(i);
            }
        }
//cout << "QQ: " << q.size() << endl;
        
        int skip = 0;
        while(skip < q.size()) {
            int k = q.front();
            q.pop_front();
            // source position.
            Pos sp = wk[k];
            // target position.
            Pos tp = sn[match1[k]];
            
            if(tp.y > sp.y && walk(k, 0) ||
                tp.x > sp.x && walk(k, 1) ||
                tp.y < sp.y && walk(k, 2) ||
                tp.x < sp.x && walk(k, 3)) {
                skip = 0;
            }
            else {
                q.push_back(k);
                ++skip;
            }
        }
        
        // move unassigned workers.
        
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] > -1) continue;    
            pair<double, int> v[4] = {};
            for(int d = 0; d < 4; ++d) {
                v[d].second = d;
            }
            for(int j = 0; j < wk.size(); ++j) {
                // push right.
                if(wk[j].y > wk[i].y) v[0].first += 1000.0 / (wk[j].y - wk[i].y);
                // push down.
                if(wk[j].x > wk[i].x) v[1].first += 1000.0 / (wk[j].x - wk[i].x);
                // push left.
                if(wk[j].x < wk[i].x) v[2].first -= 1000.0 / (wk[j].y - wk[i].y);
                // push up.
                if(wk[j].x < wk[i].x) v[3].first -= 1000.0 / (wk[j].x - wk[i].x);
            }
            sort(v, v + 4);
            for(int d = 0; d < 4; ++d) {
                dir[i][d] = v[d].second;
            }
        }
        
        q.clear();
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] == -1) {
                q.push_back(i);
            }
        }
        
        skip = 0;
        while(q.size() && skip <= q.size()) {
            int k = q.front();
            q.pop_front();
            Pos p = wk[k];
            /*
            bool found = false;
            for(int dr = 0; dr < 4 && !found; ++dr) {
                int d = dir[k][dr];    
                Pos q(p.x+dx[d], p.y+dy[d]);
                if(q.inside(n) && w[q.x][q.y] == -1) {
                    walk(k, d);
                    found = true;
                    skip = 0;
                }    
            }
            if(!found) {
                q.push_back(k);
                ++skip;     
            }        
            /*/
            int mov_d = -1;
            int nearest = 9999;
            //*
            for(int i = 0; i < wk.size(); ++i) {
                if(i == k) continue;
                else nearest = min(nearest, dist(p, wk[i]));
            }
            //*/
            random_shuffle(dk, dk+4);
            for(int z = 0; z < 4; ++z) {
                int d = dk[z];
                p.x += dx[d];
                p.y += dy[d];
                if(p.inside(n) && w[p.x][p.y] == -1) {
                    int radius = 9999;
                    for(int i = 0; i < wk.size(); ++i) {
                        if(i == k) continue;
                        else radius = min(radius, dist(p, wk[i]));
                    }
                    if(radius > nearest) {
                        mov_d = d;
                        nearest = radius;
                    }
                }
                p.x -= dx[d];
                p.y -= dy[d];
            }
            if(mov_d > -1 && walk(k, mov_d)) {
                skip = 0;
            }
            else {
                q.push_back(k);
                ++skip; 
            }
            //*/
        }
//*/

        // hire workers.
        for(int i = 0, x, y; i < sn.size() && mw > wk.size(); ++i) {
            x = sn[i].x;
            y = sn[i].y;
            if(w[x][y] == -1) {
                hire(x, y);
            }
        }
        
        
        // clean after all moves.
        for(int i = 0; i < wk.size(); ++i) {
            clean(wk[i].x, wk[i].y);
        }
        
        return ans;
    }
};



int main() {
    srand(time(0));
    SnowCleaning sc;
    
    ofstream fcase("case.txt");
    
    int n, s, c;
    cin >> n >> s >> c;
    //fout << n << ' ' << s << ' ' << c;
    
    sc.init(n, s, c);
    
    for(int i = 2000; i--;) {
        int cnt;
        cin >> cnt;
        fcase << cnt << endl;
        vector<int> v(cnt*2);
        for(int j = 0; j < cnt*2; j++) {
            cin >> v[j];
            fcase << v[j] << ' ';
        }
        fcase << endl;
        vector<string> ans = sc.nextDay(v);
        cout << ans.size() << endl;
        for(int j = 0; j < ans.size(); ++j) {
            cout << ans[j] << endl;
        }
    }
    
    return 0;
}
