#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

// direction
const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};
const char dd[] = "RDLU";


class FixTheFence {
    
    
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
    int hh[101][100];
    int vv[100][101];
    
    // back trace 
    // back trace map, records the trace step index
    int bb[101][101];
    // for bfs.
    int pr[101][101];
    // back trace path
    // 0x0FFF0FFF, x = (bt>>16), y = (bt&0xFFFF) 
    vector<int> bt;
    
    // zip and unzip the axis
    inline int zip(const int& xx, const int& yy) { return (xx<<16)|yy; }
    inline int ux(const int& zz) { return zz>>16; }
    inline int uy(const int& zz) { return zz&0xFFFF; }
    inline bool inside(const int& xx, const int& yy) { return xx>=0&&xx<=n&&yy>=0&&yy<=m;}
    
    // current position
    int x, y;
    // begin position
    int cx, cy;
    
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
        memset(pr, -1, sizeof(pr));
        bt.clear();
        path.clear();
        
        // set initial point
        cx = x = x0;
        cx = y = y0;
        
        cc[x][y] = 0;
        bb[x][y] = 0;
        bt.push_back(zip(x,y));
    }
    
    // evaluate the score in the range.
    int eval(int xa, int xb, int ya, int yb) {
        // bound the range
        xa = max(xa, 0);
        ya = max(ya, 0);
        xb = min(xa, n-1);
        yb = min(yb, m-1);
        // test
        int score = 0;
        for(int i = xa; i <= xb; ++i) {
            for(int j = y; j <= yb; ++j) {
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
                            score += 3;
                        }
                        break;
                    case 1:
                        if(edges == 1) {
                            score += 3;
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
                            score += 3;
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
                            score += 3;
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
        
//        return "0 0 RDLU";
        
        __initialize(diagram);
        
        __reset(3,3);
        
        while(true) {

cout << path << endl << endl;

for(int i = 0; i <= n; ++i) {
for(int j = 0; j <= m; ++j) {
    cout << char(cc[i][j]<0?'.':'0'+cc[i][j]);
}cout << endl;
}
system("pause");

            // key: score, value: direction.
            vector<pair<int, int> > vd(0);
            
            int x0 = x, y0 = y;
            
            // try the four direction (in fact 3 at most), and evaluate
            // the estimated score.
            for(int d = 0; d < 4; ++d) {
                x += dx[d];
                y += dy[d];
                
                if(inside(x, y) && cc[x][y] < 2) {
                    
                    // solution founded.
                    if(cc[x][y] == 1 && path.size() > 1) {
cout << "solution found!!!" << endl;
////////////////////////////////////////////////
                    }
                    else {
                        cc[x0][y0] += 1;
                        cc[x][y] += 1;
                        switch(d) {
                            case 0: // R
                                hh[x0][y0] = true;
                                break;
                            case 1: // D
                                vv[x0][y0] = true;
                                break;
                            case 2: // L
                                hh[x][y] = true;
                                break;
                            case 3: // U
                                vv[x][y] = true;
                                break;
                        } 
                        int score = eval(x0-4, x0+3, y0-4, y0+3);
                        vd.push_back(make_pair(score, d));
    cout << "score: " << score << ", dir: " << dd[d] << endl;
                        
                        switch(d) {
                            case 0: // R
                                hh[x0][y0] = false;
                                break;
                            case 1: // D
                                vv[x0][y0] = false;
                                break;
                            case 2: // L
                                hh[x][y] = false;
                                break;
                            case 3: // U
                                vv[x][y] = false;
                                break;
                        }
                        cc[x0][y0] -= 1;
                        cc[x][y] -= 1;
                    }
                }
                
                x -= dx[d];
                y -= dy[d];
            }
            
            // sort by the score, iterate from the highest score,
            // then foreach, check the backtrack path, if valid, go.
            // observated that, we go only when backtrack path exists,
            // so at least one back step exists next, so we will not
            // need to backtrack this go.
            sort(vd.rbegin(), vd.rend());
            
            // record the cells to be erased from bb after search
            vector<int> er;
            // bfs the bb to get the backtrace
            deque<int> q;
            
            bool yes = false;
            
            for(int k = 0; k < vd.size(); ++k) {
                int d = vd[k].second;
                x += dx[d];
                y += dy[d];
                
                // bfs initialize
                q.clear();
                er.clear();
                
                q.push_back(zip(x, y));
                pr[x][y] = zip(x, y);
                er.push_back(zip(x, y));
                

                
                while(!q.empty() && !yes) {
/*
cout << dd[d] << endl;
for(int i = 0; i <= n; ++i) {
for(int j = 0; j <= m; ++j) {
    cout << char(pr[i][j]==-1?'.':'#');
}cout << endl;
}
system("pause");
*/
                    int zz = q.front();
                    q.pop_front();
                    int xx = ux(zz), yy = uy(zz), xx1, yy1;

                    for(int dr = 0; dr < 4; ++dr) {
                        xx1 = xx + dx[dr];
                        yy1 = yy + dy[dr];
                        // directly backward is not allowed.
                        if(xx == x && yy == y && xx1 == x0 && yy1 == y0) continue;

                        if(inside(xx1, yy1) 
                                && pr[xx1][yy1] == -1
                                && cc[xx1][yy1] < 2) {
                            // trace founded       
                            if(bb[xx1][yy1] != -1) {
//cout << "found! : " << endl;
//printf("%d %d\n", xx, yy);
//system("pause");

                                // erase the trace rear
                                int ss = bb[xx1][yy1];
//cout << "ss = " << ss << endl;
                                while(bt.size() > ss+1) {

//printf("%d %d\n", ux(bt.back()), uy(bt.back()));

for(int i = 0; i <= 5; ++i) {
for(int j = 0; j <= 5; ++j) {
    cout << char(bb[i][j]==-1?'.':'0'+bb[i][j]);
}cout << endl;
}
                                    bb[ux(bt.back())][uy(bt.back())] = -1;
                                    bt.pop_back();

                                }
//for(int i = 0; i < bt.size(); ++i) printf("(%d, %d) ", ux(bt[i]), uy(bt[i]));cout << endl;
//system("pause");
                                // reconnect the trace
                                xx1 = xx;
                                yy1 = yy;
                                while(pr[xx1][yy1] != zip(xx1, yy1)) {
                                    // save the path
                                    bb[xx1][yy1] = bt.size();
                                    bt.push_back(zip(xx1, yy1));
                                    // move next
                                    int zz1 = pr[xx1][yy1];
                                    xx1 = ux(zz1);
                                    yy1 = uy(zz1);
//printf("zz(%d) %d %d; ", zz1, xx1, yy1);
//system("pause");
                                }
                                yes = true;
                                while(!er.empty()) {
                                    pr[ux(er.back())][uy(er.back())] = -1;
                                    er.pop_back();
                                }
                                break;
                            }
                            // deep in
                            else {
                                q.push_back(zip(xx1,yy1));
                                pr[xx1][yy1] = zip(xx, yy);
                                er.push_back(zip(xx1, yy1));
                            }
                        }
                    }
                }
                
//////////////////////////////////////////////
                x -= dx[d];
                y -= dy[d];
                
                // confirm next step..
                if(yes) {
                    x += dx[d];
                    y += dy[d];
                    cc[x0][y0] += 1;
                    cc[x][y] += 1;
                    path += dd[d];
                    switch(d) {
                        case 0: // R
                            hh[x0][y0] = true;
                            break;
                        case 1: // D
                            vv[x0][y0] = true;
                            break;
                        case 2: // L
                            hh[x][y] = true;
                            break;
                        case 3: // U
                            vv[x][y] = true;
                            break;
                    } 
//cout << (yes?"yes":"no") << endl;
/*
for(int i = 0; i < bt.size(); ++i) printf("(%d, %d) ", ux(bt[i]), uy(bt[i]));cout << endl;
for(int i = 0; i <= n; ++i) {
for(int j = 0; j <= m; ++j) {
    cout << char(bb[i][j]==-1?'.':'0'+bb[i][j]);
}cout << endl;
}
system("pause");
*/
                    break;
                }
            }
            
            // if no more solution, exit.
            if(!yes) break;
        }
        
    }
};

// tester
#include <fstream>

int main() {
    
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
    
    return 0;
    
}
