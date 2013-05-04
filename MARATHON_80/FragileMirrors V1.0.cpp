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


class Clock {
public:
    static double start;
    
    static void init() {
        start = clock();
    }
    
    static double elapsed() {
        return (clock() - start) / CLOCKS_PER_SEC;
    }
};

double Clock::start = 0;

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
    
    // ��ĳ��λ�ûָ�һ�澵�� 
    void restore(int x, int y) {
        
        #if DEBUG 
        // ���ԣ��ָ���λ���Ƿ��Ѿ�û�о����� 
        if(b[x][y]) {
            cerr << "������ͼ���о��ӵĵط��ָ�һ�澵�ӡ�" << endl;
            return;
        }
        #endif
        
        // �ָ����ӱ�� 
        b[x][y] = true;
        ++cnt;
        
        // ά������ 
        p[0][x][p[2][x][y]] = y;
        p[1][p[3][x][y]][y] = x;
        p[2][x][p[0][x][y]] = y;
        p[3][p[1][x][y]][y] = x;
        
    } 
    
    // �����ڸ�����λ�����һ�����ߵĻ�����ȥ��������
    // rollback ����ִ�к��Ƿ�ظ�ԭ״  
    int exec(int x0, int y0, bool rollback = true, int depth = 0) {
        
        #if DEBUG
        //fprintf(stderr, "��ʼ���Թ��ߣ�(%d, %d)��", x0, y0);
        #endif
        
        // 0. ����ֵ
        int ans = 0; 
        
        // 1. ��ǰ���� (x, y) / ��ǰ���� d
        int x = x0, y = y0, d;
        if(y==0) d = 0;
        else if(x==0) d = 1;
        else if(y==n+1) d = 2;
        else if(x==n+1) d = 3;
        
        // 2. ��ȥ�켣�����ڻ��� 
        vector<int> vx(0), vy(0); 
        
        // 3. ����ǰ�� 
        while(true) {
            
            #if DEBUG
            //fprintf(stderr, "(%d, %d): %c)|", x, y, dc[d]);
            #endif
            
            // 3.1. ת�Ƶ��µ�λ�� 
            x = dx[d] ? p[d][x][y] : x;
            y = dy[d] ? p[d][x][y] : y;
            
            // 3.2. ����˳�����
            if(x==0 || y==0 || x>n || y>n) break;
            
            // 3.3. ɾ������
            #if debug
            // ���ԣ����ӱ�������ڸ�λ�á� 
            if(!b[x][y]) {
                cerr << "������ͼ��û�о��ӵĵط���ȥһ�澵�ӡ�" << endl;
                break;
            }
            #endif
            
            // 3.3.1. ɾ�����ӱ��  
            b[x][y] = false;
            cnt -= 1;
            ans += 1;
            
            // 3.3.2. ά������
            p[0][x][p[2][x][y]] = p[0][x][y];
            p[1][p[3][x][y]][y] = p[1][x][y];
            p[2][x][p[0][x][y]] = p[2][x][y];
            p[3][p[1][x][y]][y] = p[3][x][y];
            
            // 3.4. ����ת�� 
            d = td[r[x][y]=='R'][d];
            
            // 3.5. ��¼�켣 
            if(rollback) {
                vx.push_back(x);
                vy.push_back(y);
            } 

        }
        
        // 3.X. ��һ�ײ���
        if(depth > 0) {
            
            // �ҵ���󻯵Ĺ���λ�� 
            int _x = -1, _y = -1, _v = 0;
            int x, y, v;
            int k = n;
            if(k > 60) k = n / 2;
            if(k > 70) k = n / 4;
            if(k > 80) k = n / 6;
            if(k > 90) k = n / 8;
            for(; k > 0; --k) {
                int i = rand() % n + 1;
                // R-ray 
                x = i; y = 0; v = exec(x, y, true, depth-1);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // D-ray 
                x = 0; y = i; v = exec(x, y, true, depth-1);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // L-ray 
                x = i; y = n+1; v = exec(x, y, true, depth-1);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // U-ray 
                x = n+1; y = i; v = exec(x, y, true, depth-1);
                if(_v < v) { _x = x; _y = y; _v = v; }
            }
            
            ans += _v / 2;
        } 
        
        // 4. �����Ҫ�ظ�ԭ״����ع����еĹ켣
        if(rollback) {
            while(!vx.empty()) {
                x = vx.back(); vx.pop_back();
                y = vy.back(); vy.pop_back();
                restore(x, y); 
            }
        } 
        
        #if DEBUG
        //fprintf(stderr, "��ȥ������%d\n", ans);
        #endif
        
        // 5. ���ؽ�� 
        return ans;
        
    }
    
};

class FragileMirrors {
public:
    vector <int> destroy(vector <string> board) {
    
        #if DEBUG
        //cerr << "program starts!" << endl;
        #endif
        
        Clock::init();
        
        // ���շ���ֵ 
        vector<int> result(0);
        
        // �������
        Mirror mi(board);
        
        #if DEBUG
        //cerr << "mirror constructed!" << endl;
        #endif
        
        // ��ʼ��ȥ
        while(mi.cnt > 0) {
            
            #if DEBUG
            //cerr << "remain: " << mi.cnt << endl;
            #endif
            
            // �ҵ���󻯵Ĺ���λ�� 
            int _x = -1, _y = -1, _v = 0;
            int x, y, v;
            int depth = Clock::elapsed() < 9 ? 1 : 0;
            for(int i = 1; i <= mi.n; ++i) {
                // R-ray 
                x = i; y = 0; v = mi.exec(x, y, true, depth);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // D-ray 
                x = 0; y = i; v = mi.exec(x, y, true, depth);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // L-ray 
                x = i; y = mi.n+1; v = mi.exec(x, y, true, depth);
                if(_v < v) { _x = x; _y = y; _v = v; }
                // U-ray 
                x = mi.n+1; y = i; v = mi.exec(x, y, true, depth);
                if(_v < v) { _x = x; _y = y; _v = v; }
            }
            
            #if DEBUG
            //cerr << "��ȥ��" << _v << endl;
            if(mi.cnt > 0 && _v == 0) {
                cerr << "�������ʧ�ܣ��޷���ȥ�κξ��ӵ����о��ӡ�" << endl;
                break; 
            }
            #endif
            
            // �������
            mi.exec(_x, _y, false);
            result.push_back(_x-1);
            result.push_back(_y-1);
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
