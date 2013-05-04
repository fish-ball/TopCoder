#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

#define DEBUG 1

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};
const char* dc = "RDLU";

class Mirror {
public:
    int n, cnt;
    char r[102][102];
    bool b[102][102];
    char p[4][102][102];
    
    Mirror(vector <string> board) {
        n = board.size();
        cnt = n * n;
        memset(b, true, sizeof(b));
        for(size_t i = 0; i < n; ++i) {
            strcpy(r[i+1]+1, board[i].c_str());
        }
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
    void restore(int x, int y) {
        
        #if DEBUG 
        // 断言：恢复的位置是否已经没有镜子了 
        if(b[x][y]) {
            cerr << "错误：试图在有镜子的地方恢复一面镜子。" << endl;
            return;
        }
        #endif
        
        // 恢复镜子标记 
        b[x][y] = true;
        ++cnt;
        
        // 维护链接 
        p[0][x][p[2][x][y]] = y;
        p[1][p[3][x][y]][y] = x;
        p[2][x][p[0][x][y]] = y;
        p[3][p[1][x][y]][y] = x;
        
    } 
    
    // 测试在给定的位置射出一条光线的话会消去几个镜子
    // rollback 返回执行后是否回复原状  
    int exec(int x0, int y0, bool rollback = true) {
        
        #if DEBUG
        fprintf(stderr, "开始尝试光线：(%d, %d)；", x0, y0);
        #endif
        
        // 0. 返回值
        int ans = 0; 
        
        // 1. 当前坐标 (x, y) / 当前方向 d
        int x = x0, y = y0, d;
        if(y==0) d = 0;
        else if(x==0) d = 1;
        else if(y==n+1) d = 2;
        else if(x==n+1) d = 3;
        
        // 2. 消去轨迹，用于回溯 
        vector<int> vx(0), vy(0); 
        
        // 3. 光线前进 
        while(true) {
            
            #if DEBUG
            fprintf(stderr, "(%d, %d): %c)|", x, y, dc[d]);
            #endif
            
            // 3.1. 转移到新的位置 
            x = dx[d] ? x : p[d][x][y];
            y = dy[d] ? y : p[d][x][y];
            
            // 3.2. 检测退出条件
            if(x==0 || y==0 || x>n || y>n) break;
            
            // 3.3. 删除镜子
            #if debug
            // 断言：镜子必须存在于该位置。 
            if(!b[x][y]) {
                cerr << "错误：试图在没有镜子的地方消去一面镜子。" << endl;
                break;
            }
            #endif
            
            // 3.3.1. 删除镜子标记  
            b[x][y] = false;
            cnt -= 1;
            ans += 1;
            
            // 3.3.2. 维护链接
            p[0][x][p[2][x][y]] = p[0][x][y];
            p[1][p[3][x][y]][y] = p[1][x][y];
            p[2][x][p[0][x][y]] = p[2][x][y];
            p[3][p[1][x][y]][y] = p[3][x][y];
            
            // 3.4. 光线转向 
            if(r[x][y] == 'R') d = (d+1)&3;
            else d = (d+3)&3;
            
            // 3.5. 记录轨迹 
            if(rollback) {
                vx.push_back(x);
                vy.push_back(y);
            } 

        }
        
        // 4. 如果需要回复原状，则回滚所有的轨迹
        if(rollback) {
            while(!vx.empty()) {
                x = vx.back(); vx.pop_back();
                y = vy.back(); vy.pop_back();
                restore(x, y); 
            }
        } 
        
        #if DEBUG
        fprintf(stderr, "消去块数：%d\n", ans);
        #endif
        
        // 5. 返回结果 
        return ans;
        
    }
    
};

class FragileMirrors {
public:
    vector <int> destroy(vector <string> board) {
    
        #if DEBUG
        cerr << "program starts!" << endl;
        #endif
        
        // 最终返回值 
        vector<int> result(0);
        
        // 构造对象
        Mirror mi(board);
        
        #if DEBUG
        cerr << "mirror constructed!" << endl;
        #endif
        
        // 开始消去
        while(mi.cnt > 0) {
            
            #if DEBUG
            cerr << "remain: " << mi.cnt << endl;
            #endif
            
            // 找到最大化的光线位置 
            int _x = -1, _y = -1, _v = 0;
            int x, y, v;
            for(int i = 1; i <= mi.n; ++i) {
                // R-ray 
                x = i; y = 0; v = mi.exec(x, y);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // D-ray 
                x = 0; y = i; v = mi.exec(x, y);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // L-ray 
                x = i; y = mi.n+1; v = mi.exec(x, y);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // U-ray 
                x = mi.n+1; y = i; v = mi.exec(x, y);
                if(_v < v) { _x = x; _y = y; _v = v; }
            }
            
            #if DEBUG
            if(mi.cnt > 0 && _v == 0) {
                cerr << "程序计算失败：无法消去任何镜子但还有镜子。" << endl;
                break; 
            }
            #endif
            
            // 射出光纤
            mi.exec(_x, _y, false);
            result.push_back(_x-1);
            result.push_back(_y-1);

        }
        
        // 返回结果
        return result; 

    }
    
};

#if DEBUG

int main() {
    
    int n;
    cin >> n;
    
    vector<string> board(n);
    for(int i = 0; i < n; ++i) {
        cin >> board[i];
        cerr << board[i] << endl;
    }
    
    FragileMirrors fm;
    
    vector<int> result = fm.destroy(board);
    
    cout << result.size() << endl;
    
    for(size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << endl;
    }
    
}

#endif
