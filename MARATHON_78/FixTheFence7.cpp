#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <ctime>
using namespace std;

// direction
const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};
const char dd[] = "RDLU";

char sbuf[10300];


class FixTheFence {
    
    double start;
    

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
    
    // records how many sides a cell has.
    int _ee[102][102];
    inline int& ee(const int& xx, const int& yy) {return _ee[xx+1][yy+1];}
    
    // records whether an edge was visited. 
    int hh[101][100];
    int vv[100][101];
    
    // back trace 
    // back trace map, records the trace step index
    int bb[101][101];
    
    // for bfs, prev node.
    int pr[101][101];
    
    // back trace path, using zip axis.
    vector<int> bt;
    
    // zip and unzip the axis
    // 0x0FFF0FFF, x = (z>>16), y = (z&0xFFFF) 
    inline int zip(const int& xx, const int& yy) { return (xx<<16)|yy; }
    inline int ux(const int& zz) { return zz>>16; }
    inline int uy(const int& zz) { return zz&0xFFFF; }
    
    // test if an crossing axis is inside the (n+1)X(m+1) grid.
    inline bool inside(const int& xx, const int& yy) { return xx>=0&&xx<=n&&yy>=0&&yy<=m;}
    
    // current position
    int x, y;
    
    // begin position
    int cx, cy;
    
    // the search path, by string "RDLU".
    string path;
    
    // initialize the grid
    void __initialize(const vector<string> &diagram) {
        // timer
        start = clock();
        
        // the diagram size
        n = diagram.size();
        m = diagram[0].size();
        
        // reset
        all = 0;
        memset(dg, -1, sizeof(dg));
        memset(cnt, 0, sizeof(cnt));
        best = 0;
        ans = "";
        
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
        memset(pr, -1, sizeof(pr));
        memset(_ee, 0, sizeof(_ee));
        
        bt.clear();
        path.clear();
        
        // set initial point
        cx = x = x0;
        cy = y = y0;
        
        // set backtrack start point.
        bb[x][y] = 0;
        bt.push_back(zip(x,y));
    }
    
    // get solution according to the cx, cy and path.
    string get_result() {
        sprintf(sbuf, "%d %d %s", cx, cy, path.c_str());
        return sbuf;
    }
    
    // commit solution and update the best.
    void commit() {
        int score = estimate();
        if(score > best) {
            best = score;
            ans = get_result();
        }
    }
    
    // move and trace as the direction.
    bool mov(int d) {
        // the next position
        int xx = x+dx[d], yy = y+dy[d];
        // next position must be inside and not fully visited.
        if(inside(xx, yy) && cc[xx][yy] < 2) {
            cc[x][y] += 1;
            cc[xx][yy] += 1;
            switch(d) {
                case 0: hh[x][y] = 1; ++ee(x-1,y); ++ee(x,y); break;
                case 1: vv[x][y] = 1; ++ee(x,y-1); ++ee(x,y); break;
                case 2: hh[xx][yy] = 1; ++ee(xx-1,yy); ++ee(xx,yy); break;
                case 3: vv[xx][yy] = 1; ++ee(xx,yy-1); ++ee(xx,yy); break;
            }
            path += dd[d];
            x = xx;
            y = yy;
            return true;
        }
        return false;
    }
    
    // retract the last move.
    bool rem() {
        int len = path.size();
        if(len > 0) {
            int d = -1;
            switch(path[len-1]) {
                case 'R': d = 0; break;
                case 'D': d = 1; break;
                case 'L': d = 2; break;
                case 'U': d = 3; break;
            }
            int xx = x - dx[d], yy = y - dy[d];
            cc[x][y] -= 1;
            cc[xx][yy] -= 1;
            switch(d) {
                case 0: hh[xx][yy] = 0; --ee(xx-1,yy); --ee(xx,yy); break;
                case 1: vv[xx][yy] = 0; --ee(xx,yy-1); --ee(xx,yy); break;
                case 2: hh[x][y] = 0; --ee(x-1,y); --ee(x,y); break;
                case 3: vv[x][y] = 0; --ee(x,y-1); --ee(x,y); break;
            }
            path.erase(len-1);
            x = xx;
            y = yy;
            return true;
        }
        return false;
    }
    
    // return the score of a given position.
    inline int pt(const int& xx, const int& yy) {
        return xx>=0&&xx<n&&yy>=0&&yy<m && dg[xx][yy]==ee(xx,yy) ? 1 : 0;
    }
    
    // return the score before a given move.
    int pt1(const int& xx, const int& yy, const int& d) {
        int xx1 = xx+dx[d], yy1 = yy+dy[d];
        switch(d) {
            case 0: return pt(xx-1,yy)+pt(xx,yy);
            case 1: return pt(xx,yy-1)+pt(xx,yy);
            case 2: return pt(xx1-1,yy1)+pt(xx1,yy1);
            case 3: return pt(xx1,yy1-1)+pt(xx1,yy1);
        }
    }
    // return the score after a given move.
    inline int pt2(const int& xx, const int& yy, const int& d) {
        int xx1 = xx-dx[d], yy1 = yy-dy[d];
        switch(d) {
            case 0: return pt(xx1-1,yy1)+pt(xx1,yy1);
            case 1: return pt(xx1,yy1-1)+pt(xx1,yy1);
            case 2: return pt(xx-1,yy)+pt(xx,yy);
            case 3: return pt(xx,yy-1)+pt(xx,yy);
        }
        
    }
    
    // estimates the final score of the current loop
    int estimate() {
        int score = 0;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < m; ++j)
                score += pt(i,j);
        return score;
    }

    // evaluate the score in the range.
    int eval(int xa, int xb, int ya, int yb) {
        int score = 0;
        for(int i = xa; i <= xb; ++i)
            for(int j = ya; j <= yb; ++j)
                score += pt(i, j);
        return score;
    }
    
    
    // evaluate the score in the range.
    int eval2(int xa, int xb, int ya, int yb) {
        // bound the range
        xa = max(xa, 0);
        ya = max(ya, 0);
        xb = min(xb, n-1);
        yb = min(yb, m-1);
        // test
        int score = 0;
        for(int i = xa; i <= xb; ++i) {
            for(int j = ya; j <= yb; ++j) {
                int edges = hh[i][j] + hh[i+1][j] + vv[i][j] + vv[i][j+1];
                int dots = (cc[i][j]>0?1:0) + (cc[i][j+1]>0?1:0)
                        + (cc[i+1][j]>0?1:0) + (cc[i+1][j+1]>0?1:0);
                bool touch = (x==i||x==i+1) && (y==j||y==j+1);
                // scoring comments:
                // 0: broken cell
                // 1: unfinished cell, not touched, unbroken
                // 2: unfinished cell, touched, unbroken
                // 3: finished cell
                switch(dg[i][j]) {
                    case 0:
                        if(edges == 0) {
                            score += 2;
                        }
                        break;
                    case 1:
                        if(edges == 1) {
                            score += 2;
                        }
                        else if(edges == 0 && dots < 4 && touch) {
                            score += 2;
                        }
                        else if(edges == 0 && dots == 0) {
                            score += 1;
                        }
                        break;
                    case 2:
                        if(edges == 2) {
                            score += 2;
                        }
                        else if(edges == 1 && dots < 4 && touch) {
                            score += 2;
                        }
                        else if(edges < 2 && dots < 4) {
                            score += 1;
                        }
                        break;
                    case 3:
                        if(edges == 3) {
                            score += 2;
                        }
                        else if(touch && (edges == 2 && dots == 3
                                    || edges == 1 && dots == 2
                                    || edges == 0 && dots == 1)) {
                            score += 2;
                        }
                        else if(!touch && dots == 0) {
                            score += 1;
                        }
                        break;
                }
            }
        }
        return score;
    }
    
    // search the backtrace path, begins with pos(x0, y0)
    // returns the shortest path solution:
    // key: break point, index of bt[].
    // value: the additional path
    pair<int, vector<int> > get_trace() {
            
        // record the cells to be erased from bb after search
        vector<int> er(0);
        // bfs the bb to get the backtrace
        // key: search depth
        // value: position zip
        deque<pair<int, int> > q(0);
        
        // return variable
        pair<int, vector<int> > ret = make_pair(-1, vector<int>(0));

        // bfs initialize
        q.push_back(make_pair(0, zip(x, y)));
        pr[x][y] = zip(x, y);
        er.push_back(zip(x, y));
            
        // the maximum backtrack(bt) index
        // because the current trying cell may overlap with it.
        int bound = bb[x][y] > -1 ? bb[x][y] : 999999;
        
        while(!q.empty()) {
            
            // unzip the information.
            int depth = q.front().first;
            int zz = q.front().second;
            int xx = ux(zz), yy = uy(zz);
            q.pop_front();
            // controlling bfs depth
            //if(depth > 4) continue;
            // search next.
            for(int d = 0; d < 4; ++d) {
                xx += dx[d];
                yy += dy[d];
                // if valid move.
                if(inside(xx, yy) && pr[xx][yy]==-1 
                        && (cc[xx][yy]==0 || cc[xx][yy]==1 && depth>0) ) {
                    // trace founded       
                    if(bb[xx][yy] != -1 && bb[xx][yy] < bound) {
                        // return the trace.
                        ret.first = bb[xx][yy];
                        xx -= dx[d];
                        yy -= dy[d];
                        while(pr[xx][yy] != zip(xx, yy)) {
                            // add node.
                            ret.second.push_back(zip(xx, yy));
                            // move next
                            zz = pr[xx][yy];
                            xx = ux(zz);
                            yy = uy(zz);
                        }
 
                        // clear the bfs marks efficiently
                        for(int i = 0; i < er.size(); ++i) {
                            pr[ux(er[i])][uy(er[i])] = -1;
                        }
                        return ret;
                    }
                    // deep in
                    else {
                        q.push_back(make_pair(depth+1, zip(xx,yy)));
                        pr[xx][yy] = zip(xx-dx[d], yy-dy[d]);
                        er.push_back(zip(xx, yy));
                    }
                }
                xx -= dx[d];
                yy -= dy[d];
            }
        }
        // clear the bfs marks efficiently
        for(int i = 0; i < er.size(); ++i) {
            pr[ux(er[i])][uy(er[i])] = -1;
        }
        // in this case, no path found, key of ret is -1.
        return ret;
    }
    
    // solution A: simple loop.
    void solve_a() {
        __reset(0, 0);
        mov(0); mov(1); mov(2); mov(3);
        commit();
    }
    
    // solution B: single line waver.
    void solve_b() {
        __reset(0, 0);
        // each row go and come.
        while(x + 2 <= n) {
            // if not the first row, down 1 unit.
            if(x > cx) mov(1);
            // move right most.
            while(y < m) mov(0);
            // down 1 unit on the right side.
            mov(1);
            // move left to 1.
            while(y > 1) mov(2);
        }
        // move left to 0.
        while(y > 0) mov(2);
        // move to top.
        while(x > 0) mov(3);
        // commit solution.
        commit();
    }
    
    // solution C: double line waver.
    void solve_c() {
        // reset start point;
        if(n % 4 != 2) __reset(1, 0);
        else __reset(0, 0);
        // each second row go and come.
        while(x + 4 <= n) {
            // if not the first row, down 2 unit.
            if(x > cx) { mov(1); mov(1); }
            // move right most.
            while(y < m) mov(0);
            // down 2 unit on the right side.
            mov(1); mov(1);
            // move left to 2.
            while(y > 2) mov(2);
        }
        // move left to 0.
        while(y > 0) mov(2);
        // move to top.
        while(x > cx) mov(3);
        // commit solution.
        commit();
    }
    

    // solution D: double line waver.
    void solve_d() {
        // reset start point;
        __reset(0, 0);
        int d = 0;
        while(mov(0));
        while(mov(1));
        while(mov(2));
        while(mov(3));
        rem();rem();
        while(true) {
            int k = 0;
            while(mov(d)) ++k;
            if(k > 1) {
                rem();
                d = (d+1)&3;
            }
            else {
                break;
            }
        }
        while(true) {
            while(mov(d));
            d = (d+3)&3;
            if(x==1&&y==0) break;
        }
        mov(3);
        commit();
    }
    
    // solve the puzzle with a given start point.
    void solve(int x_start, int y_start) {
        __reset(x_start, y_start);
        while(true) {
            // key: score, value: direction.
            int mx_score = INT_MIN;
            int mx_rep = 0; 
            int mx_len = -1;
            int mx_d = -1;
            int mx_chs = 0;
            pair<int, vector<int> > trace;
            
            int x0 = x, y0 = y;
            
            // follow the score.
            int score = 0;

            // try the four direction (in fact 3 at most), and evaluate
            // the estimated score.
            for(int d = 0; d < 4; ++d) {
                
                int rep = 0;
                int chs = 0;
                
                score -= pt1(x,y,d);
                // try the direction.
                if(mov(d)) {
                    score += pt2(x,y,d);
                    // solution founded.
                    if(cc[x][y] == 2) commit();
                    else {
                        int score_d = INT_MIN;
                        // trace two steps, get the two step score, but move one step.
                        for(int d2 = 0; d2 < 4; ++d2) {
                            if(d2-d==2||d-d2==2) continue;
                            score -= pt1(x,y,d2);
                            if(mov(d2)) {
                                score += pt2(x,y,d2);
                                if(cc[x][y] == 2) commit();
                                for(int d3 = 0; d3 < 4; ++d3) {
                                    if(d2-d3==2||d3-d2==2) continue;
                                    score -= pt1(x,y,d3);
                                    if(mov(d3)) {
                                        score += pt2(x,y,d3); 
                                        mx_chs = max(mx_chs, ++chs);
                                        if(cc[x][y] == 2) commit();
                                        if(score_d == score) {
                                            mx_rep = max(mx_rep, ++rep);
                                            
                                        }
                                        else if(score_d < score) {
                                            score_d = score;
                                            rep = 0;
                                        }
                                        score -= pt2(x,y,d3);
                                        rem();
                                    }
                                    score += pt1(x,y,d3);
                                }  
                                score -= pt2(x,y,d2);
                                rem();
                            }
                            score += pt1(x,y,d2);
                        }
                        if(score_d > mx_score || 
                            score_d == mx_score && chs == mx_chs || 
                            score_d == mx_score && chs < mx_chs && rep > mx_rep) {
                            // find path next.
                            pair<int, vector<int> > tc = get_trace();
                            if(tc.first > -1) {
                                int len = tc.first + tc.second.size();
                                if(score_d>mx_score || score_d==mx_score && len>mx_len) {
                                    mx_score = score_d;
                                    mx_len = len;
                                    trace = tc;
                                    mx_d = d;
                                }
                            }
                        }
                    }
                    
                    // retract the try.
                    score -= pt2(x,y,d);
                    rem();
                }
                score += pt1(x,y,d);
            }
            
            // if no more solution, exit.
            if(mx_score == INT_MIN) break;
            
            // move the step.
            mov(mx_d);
            
            // regenerate the back trace.
            // remove the rear
            while(bt.size() > trace.first+1) {
                int zz = bt.back();
                bb[ux(zz)][uy(zz)] = -1;
                bt.pop_back();
            }
            // connect it.
            for(int i = 0; i < trace.second.size(); ++i) {
                int zz = trace.second[i];
                int xx = ux(zz), yy = uy(zz);
                bb[ux(zz)][uy(zz)] = bt.size();
                bt.push_back(zz);
            }
        }
    }
    
    
    // solve the puzzle with a given start point.
    void solve2(int x_start, int y_start) {
        
        __reset(x_start, y_start);

        while(true) {
            // key: score, value: direction.
            int mx_score = INT_MIN;
            int mx_len = -1;
            int direction = -1;
            pair<int, vector<int> > trace;
            
            int x0 = x, y0 = y;
            
            // try the four direction (in fact 3 at most), and evaluate
            // the estimated score.
            for(int d = 0; d < 4; ++d) {
                // try the direction.
                if(mov(d)) {
                    // solution founded.
                    if(cc[x][y] == 2) {
                        // commit the solution.
                        commit();
                    }
                    else {
                        // evaluate score first.
                        int score = 0;
                        // trace two steps, get the two step score, but move one step.
                        for(int d2 = 0; d2 < 4; ++d2) {
                            if(d2-d==2||d-d2==2) continue;
                            if(mov(d2)) {
                                score = max(score, eval2(x0-3, x0+4, y0-3, y0+4));
                                rem();
                            }
                        }
                        if(score >= mx_score) {
                            // find path next.
                            pair<int, vector<int> > tc = get_trace();
                            if(tc.first > -1) {
                                int len = tc.first + tc.second.size();
                                if(score>mx_score || score==mx_score && len>mx_len) {
                                    mx_score = score;
                                    mx_len = len;
                                    trace = tc;
                                    direction = d;
                                }
                            }
                        }
                    }
                    // retract the try.
                    rem();
                }
            }
            
            // if no more solution, exit.
            if(mx_score == INT_MIN) break;
            
            // move the step.
            mov(direction);
            
            // regenerate the back trace.
            // remove the rear
            while(bt.size() > trace.first+1) {
                int zz = bt.back();
                bb[ux(zz)][uy(zz)] = -1;
                bt.pop_back();
            }
            // connect it.
            for(int i = 0; i < trace.second.size(); ++i) {
                int zz = trace.second[i];
                int xx = ux(zz), yy = uy(zz);
                bb[ux(zz)][uy(zz)] = bt.size();
                bt.push_back(zz);
            }
        }
    }
    
    // solve the puzzle with a given start point.
    void solve3(int x_start, int y_start) {
        __reset(x_start, y_start);

        while(true) {
            // key: score, value: direction.
            int mx_score = INT_MIN;
            int mx_len = -1;
            int mx_d = -1;
            pair<int, vector<int> > trace;
            
            int x0 = x, y0 = y;
            
            // follow the score.
            int score = 0;

            // try the four direction (in fact 3 at most), and evaluate
            // the estimated score.
            for(int d = 0; d < 4; ++d) {
                
                int score_d = INT_MIN;
                
                score -= pt1(x,y,d);
                // try the direction.
                if(mov(d)) {
                    score += pt2(x,y,d);
                    // solution founded.
                    if(cc[x][y] == 2) {
                        // commit the solution.
                        commit();
/////////////////end solution/////////////////////
                    }
                    else {
                        // trace two steps, get the two step score, but move one step.
                        for(int d2 = 0; d2 < 4; ++d2) {
                            if(d2-d==2||d-d2==2) continue;
                            score -= pt1(x,y,d2);
                            if(mov(d2)) {
                                score += pt2(x,y,d2);
                                for(int d3 = 0; d3 < 4; ++d3) {
                                    if(d2-d3==2||d3-d2==2) continue;
                                    score -= pt1(x,y,d3);
                                    if(mov(d3)) {
                                        score += pt2(x,y,d3);
                                        score_d = max(score_d, score);
                                        score -= pt2(x,y,d3);
                                        rem();
                                    }
                                    score += pt1(x,y,d3);
                                }
                                score -= pt2(x,y,d2);
                                rem();
                            }
                            score += pt1(x,y,d2);
                        }
                        
                        if(score_d >= mx_score) {
                            // find path next.
                            pair<int, vector<int> > tc = get_trace();
                            if(tc.first > -1) {
                                int len = tc.first + tc.second.size();
                                if(score>mx_score || score==mx_score && len>mx_len) {
                                    mx_score = score;
                                    mx_len = len;
                                    trace = tc;
                                    mx_d = d;
                                }
                            }
                        }
                    }
                    
                    // retract the try.
                    score -= pt2(x,y,d);
                    rem();
                }
                score += pt1(x,y,d);
            }
            
            // if no more solution, exit.
            if(mx_score == INT_MIN) break;
            
            // move the step.
            mov(mx_d);
            
            // regenerate the back trace.
            // remove the rear
            while(bt.size() > trace.first+1) {
                int zz = bt.back();
                bb[ux(zz)][uy(zz)] = -1;
                bt.pop_back();
            }
            // connect it.
            for(int i = 0; i < trace.second.size(); ++i) {
                int zz = trace.second[i];
                int xx = ux(zz), yy = uy(zz);
                bb[ux(zz)][uy(zz)] = bt.size();
                bt.push_back(zz);
            }
        }
    }

public:
    
    string findLoop(const vector<string> &diagram) {
        
        srand(time(0));
        
        __initialize(diagram);
        
        // ordinary try.
        solve_a();
        solve_b();
        solve_c();
        solve_d();
        
        while(double(clock() - start) / CLOCKS_PER_SEC < 9) {
            solve(rand() % n, rand() % m);
            if(double(clock() - start) / CLOCKS_PER_SEC < 9) 
                solve2(rand() % n, rand() % m);
            if(double(clock() - start) / CLOCKS_PER_SEC < 9)
                solve3(rand() % n, rand() % m);
        }
        return ans;
    }
};
// tester

int main() {
    
    srand(time(0));
    int SZ;
    
    cin >> SZ;
    
    vector<string> diagram(SZ);
    
    //ofstream fout("input.txt");
    //fout << SZ << endl;
    
    for(int i = 0; i < SZ; ++i) {
        cin >> diagram[i];
    //    fout << diagram[i] << endl;
    }
    
    FixTheFence ff;
    cout << ff.findLoop(diagram) << endl;
    
    while(1);

    return 0;
    
}
