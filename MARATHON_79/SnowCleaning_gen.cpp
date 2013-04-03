/*
STRATEGY:

1. WORKER COUNT LIMIT
    We estimates at most how many worker is the best.
    So we store $mw, to tell the maximum workers.
    When we move time to time, the maximum can be extend to: 
        (sna / (t - sun)) * co = mw * sa
        i.e.: mw = max(mw, sna * co / t / sa)
        
   IMPROVEMENT:
   
    The previous balance equation stands for average snow fall cost
   should equal the total salary per day.
   
    Finding that if the case when the grid is large, and the snows are
   pretty few, then workers will be few too, it makes the time for the
   worker move to the snow is very long, and the remaining snow cost
   will be large. So we must improve.
   
        
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
  
   IMPROVEMENT:
    
    The snow are divided into several blocks, If we match all snow cell
  equivalently, we may assign too much near workers to a same block, but
  neglecting some far blocks.
    So we should improve the matching weight function. Try to take a 
  certain amount of several snow cell in each block, and make the selected
  snow cell much more near to every workers. so the match will first deal
  with the selected blocks, then each block will get a match.
  
5. MOVE
    Each assigned worker will move to the target, with direction under
  priority RDLU.

6. UNASSIGN WORKER ACTION
    If some worker is not assigned, he will choose a direction to move,
  such that the destination position have a maximum space with the near
  most worker.
    i.e. to make the unassigned workers disperse.

*/

////////////////////////////////////////////////////
//              并查集模板(UFS) v1.2              //
////////////////////////////////////////////////////
//                                                //
// 1. SZ:         并查集最大元素总数(模板参数);   //
// 2. make_set:   初始化 N 个元素的并查集;        //
// 3. find_set:   找出某个元素所在的集合;         //
// 4. union_set:  合并两个集合;                   // 
// 5. get_sets:   返回并查集子集的个数;           //
// 6. get_size:   返回元素所在子集的大小;         //
//                                                //
// v1.2 新特性：                                  // 
//   启发式策略： 按秩合并、路径压缩              //
//   新增接口：   子集个数、子集大小              // 
//                                                //
////////////////////////////////////////////////////
//            2008 CopyRight(c) by elf            //
////////////////////////////////////////////////////

template<int SZ> class UFS {

    int p[SZ+1], rank[SZ+1], size[SZ+1], sets; 

public:
    
    void make_set( int sz ) {
        for( int i = 0; i <= sz; ++i ) {
            p[i] = i;
            rank[i] = 0;
            size[i] = 1;
        }
        sets = sz;
    }
    
    int find_set( int x ) {
        if( x != p[x] )
            p[x] = find_set( p[x] );
        return  p[x];
    }

    bool union_set( int x, int y ) {
        x = find_set( x );
        y = find_set( y );
        if( x == y )    return  false;
        --sets;
        if( rank[x] > rank[y] ) {
            size[x] += size[y];
            p[y] = x;
        }
        else {
            size[y] += size[x];
            p[x] = y;
        }
        if( rank[x] == rank[y] )
            ++rank[y];
        return  true;
    }
    
    int get_sets() const { return sets; }
    
    int get_size( int x ) { return size[find_set( x )]; }
    
};

////////////////////////////////////////////////////
//            2008 CopyRight(c) by elf            //
////////////////////////////////////////////////////


//二分图最佳匹配,kuhn munkras算法,邻接阵形式,复杂度O(m*m*n)
//返回最佳匹配值,传入二分图大小m,n和邻接阵mat,表示权值
//match1,match2返回一个最佳匹配,未匹配顶点match值为-1
//一定注意m<=n,否则循环无法终止
//最小权匹配可将权值取相反数
#include <string.h>
#define MAXN 2500
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

ofstream flog("log.txt");
ofstream fout("out.txt", ios::app);

const char* dc = "RDLU";
const int dx[] = {0, 1, 0, -1, 
                1, 1, -1, -1,
                0, 2, 0, -2,
                2, 2, -2, -2, 1, 1, -1, -1, 
                0, 3, 0, -3};
const int dy[] = {1, 0, -1, 0,
                1, -1, 1, -1,
                2, 0, -2, 0,
                1, -1, 1, -1, 2, -2, 2, -2, 
                3, 0, -3, 0};
int ibuf[2500];
char sbuf[20];

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

bool operator < (const Pos& a, const Pos& b) {
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

class SnowCleaning {
    
    
    int mat[100][2500];
    int match1[100];
    int match2[2500];
    UFS<2500> ufs;


    
    // boardSize
    int n;
    // time
    int t;
    int sun;
    // total snow fall accumulated
    int sna;
    // average snow remain;
    int snf;
    // max worker estimation
    int mw;
    // salary
    int sa;
    // snowFine cost
    int co;
    // block size, maximum block size.
    double bsz, mbsz;
    int absz, bcnt;
    
    int score;

    int dist(Pos a, Pos b) {
        return abs(a.x-b.x)+abs(a.y-b.y);
    }
    
    // workers
    int w[50][50];
    vector<Pos> wk;
    // snow
    int s[50][50];
    vector<Pos> sn;

/*
    // the block shift of a snow cell.
    vector<int> sft;
*/
    // worker processing queue
    deque<int> q;
    
    // result pool
    vector<string> ans;
    
    // sort the snow cell sequence.
    bool snow_sort() {
        memset(s, -1, sizeof(s));
        sort(sn.begin(), sn.end());
        for(int i = 0; i < sn.size(); ++i) {
            s[sn[i].x][sn[i].y] = i;
        }
    }
    
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
    // optimize maximum worker number.
    int omw() {
        int idx = 0;
        double best = 1e+8;
        for(int ww = 1; ww <= 100; ++ww) {
            double d = n / sqrt(ww*2.0);
            double rem = mbsz;
            double tt = d;
            int k = 1;
            double cost = rem * tt;
            while(rem > 0) {
                for(int i = 0; i < d; ++i) {
                    tt += min(d-i, 1.0);
                    rem -= 4.0 * k * d * min(d-i, 1.0);
                    if(rem < 0) break;
                    cost += rem * min(d-i, 1.0) * co;
                }
                k++;
            }
/*
if(t == 500) {
flog << "ww = " << ww << ", "; 
flog << "tt = " << tt << ", ";
flog << "costA = " << cost << ", ";
flog << "costB = " << ww * sa * tt << endl;
}
*/
            cost += ww * sa * tt;
            if(cost < best) {
                best = cost;
                idx = ww;
            }
        }
        return idx;
    }
    
    int init(int boardSize, int salary, int snowFine) {
        // Initialize the parameters.
        n = boardSize;
        t = 0;
        sun = 0;
        sna = 0;
        snf = 0;
        sa = salary;
        co = snowFine;
        score = 0;
        // we guess the initial block size to be 9, (3x3)
        bsz = mbsz = 9.0;
        absz = bcnt = 0;
        //mw = n * co / 4 / sa;
        //mw = 1;
        ifstream fwk("wk.txt");
        fwk >> mw;
        fwk.close();
        memset(w, -1, sizeof(w));
        wk.clear();
        memset(s, -1, sizeof(s));
        sn.clear();
        
        fout << mw << '\t' << n << '\t' << co << '\t' << sa << '\t' ;
        
        return 0;
    }
    vector<string> nextDay(vector<int> snowFalls) {

        // result pool.
        ans.clear();

        // new snow fall count.
        int k2 = snowFalls.size(), k = (k2>>1);

        // estimates the max workers.
        sna += k;
        
        // moving average snowfall of 5 days.
        snf = (snf * 19 + sn.size()) / 20;
        t += 1;
        sun += k2 == 0 ? 1 : 0;
        
        // optimize the mw
        //mw = max(mw, omw());
        
        
        //mw = max(mw, sna * co / ((t - sun) * sa));
/*
        if(mw * sa < snf * co) {
            ++mw;
        }
        mw = min(mw, n * n / 50);
//flog << mw*sa << ' ' << sn.size() * co << endl;
*/
        //mw = max(mw, snf * co / sa / 3);
        //mw = min(mw, snf);
        mw = min(mw, 100);

        // snow falls.
        for(int i = 0; i < k2; i += 2) {
            fall(snowFalls[i], snowFalls[i+1]);
        }
        snow_sort();
        
        // grouping blocks.
        ufs.make_set(k);
        
        for(int i = 0; i < k; ++i) {
            for(int j = i + 1; j < k; ++j) {
                if(dist(Pos(snowFalls[(i<<1)], snowFalls[(i<<1)+1]),
                    Pos(snowFalls[(j<<1)], snowFalls[(j<<1)+1])) < 5) {
                    ufs.union_set(i, j);
                    break;
                }
            }
        }
        
        // update the block sizes.
        for(int i = 0; i < k; ++i) {
            if(ufs.find_set(i) == i) {
                bsz = (bsz * 4 + ufs.get_size(i)) / 5.0;
                mbsz = max(mbsz, bsz);
                absz += ufs.get_size(i);
                bcnt ++;
            }
        }
        

/*
flog << "sets: " << ufs.get_sets() << endl;
for(int i = 0; i < sn.size(); ++i) {
    flog << ufs.find_set(i) << ' ';
}flog << endl;
 */
/*
        memset(ibuf, 0, sizeof(ibuf));
        sft.assign(sn.size(), 0);
        for(int i = 0; i < sn.size(); ++i) {
            sft[i] = ibuf[ufs.find_set(i)]++;
        }
*/
        
        
        
        
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
        if(wk.size() <= sn.size()) {
            for(int i = 0; i < wk.size(); ++i) {
                for(int j = 0; j < sn.size(); ++j) {
                    int dst = dist(wk[i], sn[j]);
                    //mat[i][j] = - dst*dst;
                    mat[i][j] = -(dst<<8)+abs(sn[j].x*2-n)+abs(sn[j].y*2-n);
                    //- (sft[ufs.find_set(j)]<<16);
                    //mat[i][j] = - dst ;
                }
            }
            kuhn_munkras(wk.size(), sn.size(), mat, match1, match2);
        }
        else {
            for(int i = 0; i < sn.size(); ++i) {
                for(int j = 0; j < wk.size(); ++j) {
                    int dst = dist(wk[j], sn[i]);
                    //mat[i][j] = - dst*dst;
                    mat[i][j] = -(dst<<8)+abs(sn[i].x*2-n)+abs(sn[i].y*2-n);
                    //- (sft[ufs.find_set(i)]<<16);
                    //mat[i][j] = - dst - (sft[i]<<16);
                }
            }
            kuhn_munkras(sn.size(), wk.size(), mat, match2, match1);
        }
        
//cout << "match1: "; for(int i = 0; i < wk.size(); ++i) cout << match1[i] << ' '; cout << endl;
//cout << "match2: "; for(int j = 0; j < sn.size(); ++j) cout << match2[j] << ' '; cout << endl;
        
        // move assigned workers.
        q.clear();
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] > -1) {
            //*
                q.push_back(i);
            /*/
                if(dist(wk[i], sn[match1[i]]) > n/2 && q.size() >= 3) {
                    match1[i] = -1;
                }
                // if it is too far, not go.
                else {
                    q.push_back(i);
                }
            //*/
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
        //*
        // move unassigned workers.
        q.clear();
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] == -1) {
                q.push_back(i);
            }
        }
        skip = 0;
        while(skip < q.size()) {
            int k = q.front();
            q.pop_front();
            Pos p = wk[k];
            int mov_d = -1;
            int nearest = 0;
            for(int d = 0; d < 4; ++d) {
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
        
        score += wk.size() * sa + sn.size() * co;
        
        if(t == 2000) {
            fout << sun << '\t' << sna << '\t' << absz / bcnt << '\t' << score << endl;
        }
        
        return ans;
    }
};


int main() {
    
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
