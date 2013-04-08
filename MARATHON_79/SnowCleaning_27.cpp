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
#include <map>
#include <algorithm>
#include <fstream>
using namespace std;

//ofstream fout("test.txt", ios::app);

const char* dc = "RDLU";
const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};
char sbuf[20];

// Pos struct
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

// Executive class
class SnowCleaning {
    
    UFS<2500> ufs;
    
    // matching temp.
    int mat[100][MAXN];
    int match1[100];
    int match2[MAXN];
    
    // the distance of the nearest cell of snow.
    int diss[50][50];
    // the distance of the nearest cell of worker.
    int disw[50][50];

    // boardSize
    int n;
    // time
    int t;
    // days of no snow
    int sun;
    // total snow fall accumulated
    double sna;
    // max worker estimation
    int mw;
    // salary
    double sa;
    // snowFine cost
    double co;
    // accumulated score
    int score;

    
    // workers
    int w[50][50];
    vector<Pos> wk;
    map<int, Pos> target;
    // snow
    int s[50][50];
    vector<Pos> sn;
    // sentinel
    vector<Pos> st;
    
    // worker processing queue
    deque<int> q;
    int skip;
    
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
        cerr << "hire: (" << x << ", " << y << ")" << endl;
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
    
    // let worker k move towards direction d.
    inline bool walk(int k, int d) {
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
    
    void cal_diss() {
        memset(diss, -1, sizeof(diss));
        deque<Pos> sq(0);
        for(int i = 0; i < sn.size(); ++i) {
            diss[sn[i].x][sn[i].y] = 0;
            sq.push_back(sn[i]);
        }
        while(!sq.empty()) {
            Pos p = sq.front();
            sq.pop_front();
            int now = diss[p.x][p.y];
            for(int d = 0; d < 4; ++d) {
                p.x += dx[d];
                p.y += dy[d];
                if(p.inside(n) && diss[p.x][p.y] == -1) {
                    diss[p.x][p.y] = now + 1;
                    sq.push_back(p);
                }
                p.x -= dx[d];
                p.y -= dy[d];
            }
        }
    }
    
    void cal_disw() {
        memset(disw, -1, sizeof(disw));
        deque<Pos> sq(0);
        for(int i = 0; i < wk.size(); ++i) {
            disw[wk[i].x][wk[i].y] = 0;
            sq.push_back(wk[i]);
        }
        while(!sq.empty()) {
            Pos p = sq.front();
            sq.pop_front();
            int now = disw[p.x][p.y];
            for(int d = 0; d < 4; ++d) {
                p.x += dx[d];
                p.y += dy[d];
                if(p.inside(n) && disw[p.x][p.y] == -1) {
                    disw[p.x][p.y] = now + 1;
                    sq.push_back(p);
                }
                p.x -= dx[d];
                p.y -= dy[d];
            }
        }
    }
    
public:
    int init(int boardSize, int salary, int snowFine) {
        // Initialize the parameters.
        score = 0;
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
        st.clear();
        return 0;
    }
    vector<string> nextDay(vector<int> snowFalls) {

        // result pool.
        ans.clear();
        target.clear();

        // new snow fall count.
        int k2 = snowFalls.size(), k = (k2>>1);

        // estimates the max workers.
        sna += k;
        t += 1;
        sun += k2 == 0 ? 1 : 0;
        
        mw = int(sqrt(sna * (0.55 + 0.4 * min(t,1000) / 1000.0) / t * co / sa) * n / 4.0);
        mw = min(mw, 100);
        //if(mw < 10) mw -= 1;

        // snow falls.
        for(int i = 0; i < k2; i += 2) {
            fall(snowFalls[i], snowFalls[i+1]);
        }
        
        
        // grouping blocks.
        ufs.make_set(sn.size());
        
        for(int i = 0; i < sn.size(); ++i) {
            for(int j = i + 1; j < sn.size(); ++j) {
                if(dist(sn[i], sn[j]) < 5) {
                    ufs.union_set(i, j);
                    break;
                }
            }
        }
        
        cal_diss();
        
        /////////////////////////
        // first assignment
        
        memset(mat, 0, sizeof(mat));
        int range = int(1.25 * n / sqrt(1.0*mw) + 7.5);
        for(int i = 0; i < wk.size(); ++i) {
            for(int j = 0; j < sn.size(); ++j) {
                int dst = dist(wk[i], sn[j]);
                // matching function.
                mat[i][j] = -(dst<<16)+abs(sn[j].x*2-n)+abs(sn[j].y*2-n);
                //mat[i][j] = -(dst<<16)+sn[j].x*256+sn[j].y;
            }
            // outside matching, to let the unassign workers to place.
            for(int j = sn.size(); j < sn.size() + wk.size(); ++j) {
                mat[i][j] = -(range<<16);
            }
        }
//        fout << t << ": " << range << endl;
        kuhn_munkras(wk.size(), sn.size()+wk.size(), mat, match1, match2);
        
        // remve the unmatch indeed.
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] >= sn.size()) 
                match1[i] = -1;
        }
        
        // cluster..
        int bsz[2500] = {}; // currently block size remained.
        int lat[2500] = {}; // currently last worker time cost.
        int wsz[2500] = {}; // currently workers arrived.
        
        // generate block size.
        for(int i = 0; i < sn.size(); ++i) {
            int j = ufs.find_set(i);
            bsz[j] = ufs.get_size(j) * 2;
        }
        
        // sort matched workers by priority.
        vector<pair<int, int> > pr(0);
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] > -1)
                pr.push_back(make_pair(dist(wk[i], sn[match1[i]]), i));
        }
        sort(pr.begin(), pr.end());
        
        // workers to be dis assigned.
        vector<int> rem_wk(0);
        
        // distance calculation.
        for(int i = 0; i < pr.size(); ++i) {
            int wi = pr[i].second;
            int tt = pr[i].first;
            int bi = ufs.find_set(match1[wi]);
            bsz[bi] -= wsz[bi] * (tt-lat[bi]);
            lat[bi] = tt;
            ++wsz[bi];
            if(bsz[bi] < 0) {
                rem_wk.push_back(wi);
            }
        }
        
        for(int i = 0; i < rem_wk.size(); ++i) {
//            match1[rem_wk[i]] = -1;
        }
        
        // move assigned workers.
        for(int i = 0; i < wk.size(); ++i) {
            if(match1[i] > -1) {
                target[i] = sn[match1[i]];
            }
        }
        
        //////////////////////////////
        // move unassigned workers.
        vector<int> uw(0);
        for(int i = 0; i < wk.size(); ++i) {
            if(target.find(i) == target.end()) {
                uw.push_back(i);
            }
        }
        /*
        vector<pair<int, int> > st_exclude(0);
        for(int i = 0; i < st.size(); ++i) {
            st_exclude.push_back(make_pair(dis[st[i].x][st[i].y], (i % 2) * 9999 + i));
        }
        random_shuffle(st_exclude.begin(), st_exclude.end());
        sort(st_exclude.begin(), st_exclude.end());
        //*/
        //////////////////////////////////
        memset(mat, 0, sizeof(mat));
        
        for(int i = 0; i < uw.size(); ++i) {
            for(int j = 0; j < st.size(); ++j) {
                int dst = dist(wk[uw[i]], st[j]);
                // matching function.
                mat[i][j] = -(dst<<20)-(dst*dst);
            }
        }
        /*
        // exclusion
        for(int j = 0; j + q.size() * 2 < st_exclude.size(); ++j) {
            for(int i = 0; i < q.size(); ++i) {
                mat[i][st_exclude[j].second % 9999] = - n * 2;
            }
        }
        //*/
        kuhn_munkras(uw.size(), st.size(), mat, match1, match2);
        
        for(int i = 0; i < uw.size(); ++i) {
            if(match1[i] > -1) {
                target[uw[i]] = st[match1[i]];
            }
        }
        /////////////////////////////////
        skip = 0;
        q.clear();
        for(map<int, Pos>::iterator it = target.begin(); it != target.end(); ++it) {
            q.push_back(it->first);
        }
        //random_shuffle(q.begin(), q.end());
        
        while(q.size() && skip <= q.size()) {
            int k = q.front();
            q.pop_front();
            // source position.
            Pos sp = wk[k];
            // target position.
            Pos tp = target[k];
            
            vector<pair<int, int> > pv(0);
            if(tp.y > sp.y) pv.push_back(make_pair(9999, 0));
            if(tp.x > sp.x) pv.push_back(make_pair(9999, 1));
            if(tp.y < sp.y) pv.push_back(make_pair(9999, 2));
            if(tp.x < sp.x) pv.push_back(make_pair(9999, 3));
            
            bool moved = false;
            
            for(int i = 0; i < pv.size(); ++i) {
                int d = pv[i].second;
                Pos pp = sp;
                pv[i].first = 0;
                while(pp.inside(n)) {
                    if(s[pp.x][pp.y]) break;
                    pp.x += dx[d];
                    pp.y += dy[d];
                    ++pv[i].first;
                }
                if(!pp.inside(n)) pv[i].first = 9999;
            }
            sort(pv.begin(), pv.end());
            for(int i = 0; i < pv.size(); ++i) {
                if(walk(k, pv[i].second)) {
                    moved = true;
                    break;
                }
            }
            
            if(moved) {
                skip = 0;
            }
            else {
                q.push_back(k);
                ++skip;
            }
        }
        
        // clean after moves.
        for(int i = 0; i < wk.size(); ++i) {
            clean(wk[i].x, wk[i].y);
        }

        // hire workers.
        bool isAnyoneHired = false;
        
        //mw = min(mw, max(4, int(wk.size() + 1 + sn.size() * co / sa / 2)));
        while(wk.size() < mw && sn.size() > 0) {
            cal_disw();
            
            Pos pp;
            int best = -1;
            
            for(int i = 0; i < sn.size(); ++i) {
                if(best < disw[sn[i].x][sn[i].y]) {
                    pp = sn[i];
                    best = disw[sn[i].x][sn[i].y];
                }
            }
            
            hire(pp.x, pp.y);
            clean(pp.x, pp.y);
            isAnyoneHired = true;
        }
        
        // if new worker is hired, regenerate the sentinels.
        if(isAnyoneHired) {
            int m1 = 0, m2 = 0;
            
            while(true) {
                if((m1 * m2 + 1) / 2 >= wk.size()) break;
                ++m1;
                if((m1 * m2 + 1) / 2 >= wk.size()) break;
                ++m2;
            }
            
            st.clear();
            
            bool odd = wk.size() == (m1 * m2 + 1) / 2;
            
            double side1 = double(n) / m1;
            double side2 = double(n) / m2;
            
            for(int i = 0; i < m1; ++i) {
                for(int j = odd ? 0 : 1; j < m2; j += 2) {
                    Pos p(int(side1*(i+0.5)), int(side2*(j+0.5)));
                    if(p.inside(n));
                    st.push_back(p);
                }
                odd = !odd;
            }
        }
        
        
        score += wk.size() * int(sa+.1) + sn.size() * int(co+.1);
        
        return ans;
    }
};

int main() {
    srand(time(0));
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
