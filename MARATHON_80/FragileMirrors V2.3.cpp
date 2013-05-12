#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <set>
using namespace std;

#define DEBUG 1

typedef unsigned long long i64;

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};
int td[2][4] = {
    {3, 2, 1, 0}, // L
    {1, 0, 3, 2}  // R
};
const char* dc = "RDLU";

inline int zip(int x, int y) {return (x<<16)|y;}
inline int zx(int z) {return z>>16;}
inline int zy(int z) {return z&0xFFFF;}
inline i64 hash_code(i64 key) {
    key = (~key) + (key << 21); // key = (key << 21) - key - 1; 
    key = key ^ (key >> 24); 
    key = (key + (key << 3)) + (key << 8); // key * 265 
    key = key ^ (key >> 14); 
    key = (key + (key << 2)) + (key << 4); // key * 21 
    key = key ^ (key >> 28); 
    key = key + (key << 31); 
    return key;
}


class Clock {
public:
    static double start;
    static void init() {start = clock();}
    static double elapsed() {return (clock() - start) / CLOCKS_PER_SEC;}
};

double Clock::start = 0;


class Mirror {
public:
    int n, cnt;
    i64 hash;
    char r[102][102];
    bool b[102][102];
    char p[4][102][102];
    
    Mirror(vector <string> board) {
        
        // 获取计数 
        n = board.size();
        cnt = n * n;
        hash = 0;
        
        // 清空镜子状态为全部存在 
        memset(b, true, sizeof(b));
        
        // 读入镜子 
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                r[i+1][j+1] = board[i][j] == 'R' ? 1 : 0;
                hash ^= hash_code(zip(i+1, j+1));
            }
        }
        
        // 使用 r 的边角位简化初始方向 
        for(int i = 1; i <= n; ++i) {
            r[i][0] = 0;
            r[0][i] = 1;
            r[i][n+1] = 2;
            r[n+1][i] = 3;
        }
        
        // 初始化十字链表 
        for(size_t i = 0; i < n+2; ++i) {
            for(size_t j = 0; j < n+2; ++j) {
                p[0][i][j] = j + 1;
                p[1][i][j] = i + 1;
                p[2][i][j] = j - 1;
                p[3][i][j] = i - 1;
            }
        }
    }
    
    // 在某个位置恢复一面镜子 
    inline void restore(int x, int y) {
        
        // 恢复镜子标记 
        b[x][y] = true;
        hash ^= hash_code(zip(x, y));
        ++cnt;
        
        // 维护链接 
        p[0][x][p[2][x][y]] = y;
        p[1][p[3][x][y]][y] = x;
        p[2][x][p[0][x][y]] = y;
        p[3][p[1][x][y]][y] = x;

    }
    
    inline void restore(int z) {
        restore(zx(z), zy(z));
    }
    
    void restore(vector<int>& path) {
        while(!path.empty()) {
            restore(path.back());
            path.pop_back();
        } 
    }
    
    // 测试在给定的位置射出一条光线的话得分是多少 
    // rollback 返回执行后是否回复原状  
    int exec(int x, int y, vector<int>& path) {
        
        // 1. 当前坐标 (x, y) / 当前方向 d
        int d = r[x][y];
        
        // 3.1. 转移到新的位置 
        x = dx[d] ? p[d][x][y] : x;
        y = dy[d] ? p[d][x][y] : y;
        
        // 3. 光线前进 
        while(x>0 && x<=n && y>0 && y<=n) {
            
            // 3.4. 光线转向 
            d = td[r[x][y]][d];
            
            // 3.5. 记录轨迹 
            path.push_back(zip(x, y));
            
            // 3.3.1. 删除镜子标记  
            b[x][y] = false;
            hash ^= hash_code(zip(x, y));
            --cnt;
            
            // 3.3.2. 维护链接
            p[0][x][p[2][x][y]] = p[0][x][y];
            p[1][p[3][x][y]][y] = p[1][x][y];
            p[2][x][p[0][x][y]] = p[2][x][y];
            p[3][p[1][x][y]][y] = p[3][x][y];

            // 3.1. 转移到新的位置 
            x = dx[d] ? p[d][x][y] : x;
            y = dy[d] ? p[d][x][y] : y;
        }
        
        return cnt;
        
    }
    
    inline int exec(int z, vector<int>& path) {
        return exec(zx(z), zy(z), path);
    }
    
    
    int exec(const vector<int> vz, vector<int>& path) {
        int ans = 0;
        for(size_t i = 0; i < vz.size(); ++i) {
            exec(vz[i], path);
        }
        return cnt;
    }
    
};

struct Status {
    int score; // 得分
    vector<int> vz; // 光线堆栈 
    Status() : score(0), vz(0) {}
    Status(const Status& s) : score(s.score), vz(s.vz) {}
};

bool operator < (const Status& lhs, const Status& rhs) {
    return lhs.score < rhs.score;
}

class FragileMirrors {
public:
    vector <int> destroy(vector <string> board) {
    
        Clock::init();
        
        // 最终返回值 
        vector<int> result(0);
        
        // 构造对象
        Mirror mi(board);
        

        set<i64> H;
        #if DEBUG
        int tt = 0;
        #endif
        // 开始消去
        while(mi.cnt > 0) {
             

            size_t K, D;
            
            if(Clock::elapsed() < 9) {
                K = 6000 / mi.n - 15;
                D = 200;
            }
            else {
                K = 10;
                D = 3;
            }
            
            #if DEBUG
            cerr<<++tt<<": "<<mi.cnt<<"\tK = " << K << endl;
            #endif
            
            H.clear();
            H.insert(mi.hash);

            vector<Status> vs(1, Status());
            
            vector<int> path(0), path2(0);
            
            int cnt0 = mi.cnt;
            
            for(int dep = 0; dep < D; ++dep) {
                if(Clock::elapsed() > 9.5 && K > 10) break;
//cerr<<"depth: " << dep << " size: " << vs.size() <<endl;
                vector<Status> vs2(0);
                for(size_t k = 0; k < vs.size(); ++k) {
                    mi.exec(vs[k].vz, path);
                    Status s;
                    for(int i = 1, v; i <= mi.n; ++i) {
//cerr<<mi.hash<<" ";
                        s = vs[k]; s.score = mi.exec(0  , i, path2); 
                        if(H.find(mi.hash) == H.end()) {
                            H.insert(mi.hash);
                            s.vz.push_back(zip(0  , i)); vs2.push_back(s); 
                        }
                        mi.restore(path2); 
//cerr<<mi.hash<<endl;
                        
                        s = vs[k]; s.score = mi.exec(i  , 0, path2); 
                        if(H.find(mi.hash) == H.end()) {
                            H.insert(mi.hash);
                            s.vz.push_back(zip(i  , 0)); vs2.push_back(s);
                        }
                        mi.restore(path2); 
                        
                        s = vs[k]; s.score = mi.exec(mi.n+1, i, path2); 
                        if(H.find(mi.hash) == H.end()) {
                            H.insert(mi.hash);
                            s.vz.push_back(zip(mi.n+1, i)); vs2.push_back(s);
                        }
                        mi.restore(path2); 
                        
                        s = vs[k]; s.score = mi.exec(i, mi.n+1, path2); 
                        if(H.find(mi.hash) == H.end()) {
                            H.insert(mi.hash);
                            s.vz.push_back(zip(i, mi.n+1)); vs2.push_back(s); 
                        }
                        mi.restore(path2); 
                    }
                    mi.restore(path);
                }
                sort(vs2.begin(), vs2.end());
                vs2.resize(min(K, vs2.size()));
                if(vs2.empty()) break;
                vs = vs2;
                if(vs[0].score == 0) break;
                int j = 1;
                for(; j < vs.size(); ++j) {
                    if(vs[j].vz[0] != vs[j-1].vz[0]) break;
                }
                if(j == vs.size()) break;
            }
            
            if(vs[0].vz.empty()) continue;
            
            for(size_t i = 0; i+i+i < vs[0].vz.size(); ++i) {
                int z = vs[0].vz[i];
                mi.exec(z, path);
    //fprintf(stderr, "(%d, %d)\n", zx(z), zy(z));
                result.push_back(zx(z)-1);
                result.push_back(zy(z)-1);
            }

        }
//cerr<<"score = " << 2.0 * mi.n / result.size() << endl;
        // 返回结果
        return result; 

    }
    
};

#if DEBUG

#include <fstream>

int main() {

    int n;
    cin >> n;

    ofstream fout("out.txt", ios::app);

    vector<string> board(n);
    for(int i = 0; i < n; ++i) {
        cin >> board[i];
        //cerr << board[i] << endl;
    }
    
    FragileMirrors fm;
    
    vector<int> result = fm.destroy(board);
    
    cout << result.size() << endl;
    
    for(size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << endl;
    }
    fout << n << '\t' << Clock::elapsed() << '\t' << n * 2.0 / result.size() << endl;
    fout.close();
}

#endif
