#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

class FixTheFence {
    
    // direction
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {1, 0, -1, 0};
    const char dd[] = "RDLU";
    
    // initial diagram graph.
    char dg[100][100];
    
    // n X m: size
    // cnt: count of the cells has each number
    // all: count of all the cells that contains number
    int n, m, cnt[4], all;
    
    // best score founded.
    int best;
    // current best answer string.
    string ans;

    // records how many times a corner was visited.
    int cc[101][101];
    
    // records whether an edge was visited. 
    bool hh[101][100];
    bool vv[100][101];
    
    // back trace 
    // back trace map, records the 
    int bb[101][101];
    // back trace path
    // 0x0FFF0FFF, x = (bt>>16), y = (bt&0xFFFF) 
    vector<int> bt;
    
    // zip and unzip the axis
    inline int zip(const int& xx, const int& yy) { return (xx<<16)|yy; }
    inline int ux(const int& zz) { return zz>>16; }
    inline int uy(const int& zz) { return zz&0xFFFF; }
    
    // current position
    int x, y;
    
    // the search path, by string "RDLU".
    string path;
    
    // initialize the grid
    void __initialize(const vector<string> &diagram) {
        
        // the diagram size
        n = diagram.size();
        m = diagram[0].size();
        
        // reset
        all = 0;
        memset(dg, -1, sizeof(dg));
        memset(cnt, 0, sizeof(cnt));
        best = 0;
        ans = "0 0 RDLU";
        
        // record
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                if(diagram[i][j] != '-') {
                    all++;
                    dg[i][j] = diagram[i][j] - '0';
                    cnt[dg[i][j]]++;
                }
            }
        }
        
    }
    
    // reset the trace
    void __reset(int x0, int y0) {
        
        // clear
        memset(cc, 0, sizeof(cc));
        memset(hh, 0, sizeof(hh));
        memset(vv, 0, sizeof(vv));
        memset(bb, -1, sizeof(bb));
        bt.clear();
        path.clear();
        
        // set initial point
        x = x0;
        y = y0;
        cc[x][y] = 1;
        bb[x][y] = 0;
        bt.push_back(zip(x,y));
    }
    
    // evaluate the 
    void eval(int xa, int xb, int ya, int yb) {
    }
    
    // solution A: simple loop.
    string solve_a() {
        return "0 0 RDLU";
    }
    
    // solution B: single line waver.
    string solve_b() {
        
        string ret = "0 0 ";
        
        int x = 0, y = 0;
        
        while(x + 2 <= n) {
            // if not begin, down 1 unit.
            if(x > 0) {
                ret += 'D';
                x += 1;
            }
            
            // move right most.
            while(y < n) {
                ret += 'R';
                y += 1;
            }
            // down 1 unit on the right side.
            ret += 'D';
            x += 1;
            // move left to 1.
            while(y > 1) {
                ret += 'L';
                y -= 1;
            }
        }
        
        while(y > 0) {
            ret += 'L';
            y -= 1;
        }
        
        while(x > 0) {
            ret += 'U';
            x -= 1;
        }
        
        return ret;
    }
    
    // solution C: double line waver.
    string solve_c() {
        
        string ret = "0 0 ";
        
        int x = 0, y = 0;
        
        while(x + 4 <= n) {
            // if not begin, down 2 unit.
            if(x > 0) {
                ret += 'D';
                ret += 'D';
                x += 2;
            }
            
            // move right most.
            while(y < n) {
                ret += 'R';
                y += 1;
            }
            // down 2 unit on the right side.
            ret += 'D';
            ret += 'D';
            x += 2;
            // move left to 2.
            while(y > 2) {
                ret += 'L';
                y -= 1;
            }
        }
        
        while(y > 0) {
            ret += 'L';
            y -= 1;
        }
        
        while(x > 0) {
            ret += 'U';
            x -= 1;
        }
        
        if(n % 4 > 0) ret[0] = '1';
        
        return ret;
    }
    
public:
    
    string findLoop(const vector<string> &diagram) {
        
        __initialize();
        
        __reset(3,3);
        
        while(true) {
            // key: score, value: direction.
            vector<pair<int, int> > vd(0);
            // record the cells to be erased from bb after search
            vector<int> er;
            // bfs the bb to get the backtrace
            deque<int> q;
            for(int d = 0; d < 4; ++d) {
                x += dx[d];
                y += dy[d];
                
                if(x >= 0 && x < n && y >= 0 && y < m) {
                    int xx = x, yy = y;
                    q.clear();
                    er.clear();
                }
                
                x -= dx[d];
                y -= dy[d];
            }
        }
        
    }
};

// tester

int main() {
    
    int SZ;
    
    cin >> SZ;
    
    vector<string> diagram(SZ);
    
    for(int i = 0; i < SZ; ++i) cin >> diagram[i];
    
    FixTheFence ff;
    cout << ff.findLoop(diagram) << endl;
    
    return 0;
    
}
