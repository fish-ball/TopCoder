#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
using namespace std;

#define DEBUG 1

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
    char r[102][102];
    bool b[102][102];
    char p[4][102][102];
    
    Mirror(vector <string> board) {
        
        // ��ȡ���� 
        n = board.size();
        cnt = n * n;
        
        // ��վ���״̬Ϊȫ������ 
        memset(b, true, sizeof(b));
        
        // ���뾵�� 
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                r[i+1][j+1] = board[i][j] == 'R' ? 1 : 0;
            }
        }
        
        // ʹ�� r �ı߽�λ�򻯳�ʼ���� 
        for(int i = 1; i <= n; ++i) {
            r[i][0] = 0;
            r[0][i] = 1;
            r[i][n+1] = 2;
            r[n+1][i] = 3;
        }
        
        // ��ʼ��ʮ������ 
        for(size_t i = 0; i < n+2; ++i) {
            for(size_t j = 0; j < n+2; ++j) {
                p[0][i][j] = j + 1;
                p[1][i][j] = i + 1;
                p[2][i][j] = j - 1;
                p[3][i][j] = i - 1;
            }
        }
    }
    
    // ��ĳ��λ�ûָ�һ�澵�� 
    inline void restore(int x, int y) {
        
        // �ָ����ӱ�� 
        b[x][y] = true;
        ++cnt;
        
        // ά������ 
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
    
    // �����ڸ�����λ�����һ�����ߵĻ��÷��Ƕ��� 
    // rollback ����ִ�к��Ƿ�ظ�ԭ״  
    int exec(int x, int y, vector<int>& path) {
        
        // 1. ��ǰ���� (x, y) / ��ǰ���� d
        int d = r[x][y];
        
        // 3.1. ת�Ƶ��µ�λ�� 
        x = dx[d] ? p[d][x][y] : x;
        y = dy[d] ? p[d][x][y] : y;
        
        // 3. ����ǰ�� 
        while(x>0 && x<=n && y>0 && y<=n) {
            
            // 3.4. ����ת�� 
            d = td[r[x][y]][d];
            
            // 3.5. ��¼�켣 
            path.push_back(zip(x, y));
            
            // 3.3.1. ɾ�����ӱ��  
            b[x][y] = false;
            --cnt;
            
            // 3.3.2. ά������
            p[0][x][p[2][x][y]] = p[0][x][y];
            p[1][p[3][x][y]][y] = p[1][x][y];
            p[2][x][p[0][x][y]] = p[2][x][y];
            p[3][p[1][x][y]][y] = p[3][x][y];

            // 3.1. ת�Ƶ��µ�λ�� 
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
    int score; // �÷�
    vector<int> vz; // ���߶�ջ 
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
        
        // ���շ���ֵ 
        vector<int> result(0);
        
        // �������
        Mirror mi(board);
        
        size_t PK[] = {30, 30, 15, 15, 15, 15, 15, 15, 7, 6};
        int PD[] =    {40, 40, 40, 40, 40, 40, 40, 40, 40, 2};
        
        // ��ʼ��ȥ
        while(mi.cnt > 0) {
            
            size_t K = PK[int(Clock::elapsed())]; // ÿ�����Ż�ȡ��״̬���� 
            int D = PD[int(Clock::elapsed())]; // ������ 
cerr<<mi.cnt<<endl;
            K = 400;
            D = 100;

            vector<Status> vs(1, Status());
            
            vector<int> path(0), path2(0);
            
            int cnt0 = mi.cnt;
            
            for(int dep = 0; dep < D; ++dep) {
                vector<Status> vs2(0);
                for(size_t k = 0; k < vs.size(); ++k) {
                    mi.exec(vs[k].vz, path);
                    //if(dep > 0 || mi.cnt < cnt0) {
                        Status s;
                        for(int i = 1, v; i <= mi.n; ++i) {
                            s = vs[k]; s.score = mi.exec(0  , i, path2); s.vz.push_back(zip(0  , i)); vs2.push_back(s); mi.restore(path2); 
                            s = vs[k]; s.score = mi.exec(i  , 0, path2); s.vz.push_back(zip(i  , 0)); vs2.push_back(s); mi.restore(path2); 
                            s = vs[k]; s.score = mi.exec(mi.n+1, i, path2); s.vz.push_back(zip(mi.n+1, i)); vs2.push_back(s); mi.restore(path2); 
                            s = vs[k]; s.score = mi.exec(i, mi.n+1, path2); s.vz.push_back(zip(i, mi.n+1)); vs2.push_back(s); mi.restore(path2); 
                        }
                    //}
                    mi.restore(path);
                }
                sort(vs2.begin(), vs2.end());
                vs2.resize(min(K, vs2.size()));
                vs = vs2;
                int j = 1;
                if(vs[0].score == 0) break;
                for(; j < vs.size(); ++j) {
                    if(vs[j].vz[0] != vs[j-1].vz[0]) break;
                }
                if(j == vs.size()) break;
            }
            
            int z = vs.front().vz.front();
            mi.exec(z, path);
            result.push_back(zx(z)-1);
            result.push_back(zy(z)-1);

        }
        
        // ���ؽ��
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
        //cerr << board[i] << endl;
    }
    
    FragileMirrors fm;
    
    vector<int> result = fm.destroy(board);
    
    cout << result.size() << endl;
    
    for(size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << endl;
    }
    
}

#endif
