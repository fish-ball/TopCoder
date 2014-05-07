#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <deque>
#include <map>
#include <set>
using namespace std;

typedef long long i64;

const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};

const int vx[] = {0, 0, 1, 1};
const int vy[] = {0, 1, 0, 1};

char G[16][16];

struct Point {
	int x, y, k;
	Point(const int &x=0, const int &y=0, const int &k=0) 
		: x(x), y(y), k(k) {}
	inline bool inside(const int& N) {
		return x>=0 && x<N && y>=0 && y<N;
	}
	void disp() {
		cerr << int(k)<<" ("<<x<<","<<y<<") : " << int(G[x][y]) << endl;
	}
};

inline bool operator ==(const Point& a, const Point& b) {
	return a.x == b.x && a.y == b.y;
}

inline bool operator !=(const Point& a, const Point& b) {
	return !(a == b);
}


// 一个消去的匹配方案
struct MatchSet {
	
	// 整个匹配所需的步数
	int dis;
	// 匹配消去的颜色
	int c;
	// 需要将这四个位置移动到目标
	int x[4], y[4];
	// 目标消去格的左上角
	Point p;

	MatchSet(int dis=0) : dis(dis) {
		memset(x, -1, sizeof(x));
		memset(y, -1, sizeof(y));
	}
	inline bool contains(int xx, int yy) {
		return x[0]==xx && y[0]==yy ||
				x[1]==xx && y[1]==yy ||
				x[2]==xx && y[2]==yy ||
				x[3]==xx && y[3]==yy;
	}
	inline bool valid() {
		return x[0]>-1 && x[1]>-1 && x[2]>-1 && x[3]>-1;
	}
	void disp() {
		cerr << "("<<p.x<<","<<p.y << ") :"<<c<<" ";
		cerr << "d-" << dis;
		for(int d = 0; d < 4; ++d) cerr << " ("<<x[d]<<","<<y[d]<<")";
		cerr << endl;
	}
};

inline bool operator < (const MatchSet& a, const MatchSet& b) {
	return a.dis < b.dis;
}

class SquareRemover {
    
    int N, C, SC, SD;
    
    //char G[16][16];

	char B[16][16];
    
    vector<int> ansv;

	int SEQ[256];
    
    
public:

	// 每调用一次获取一个新的填充框
	int get_tile() {
		int color = SD % C;
		SD = (SD * 48271LL) % 2147483647LL;
		return color;
	}

	// 返回要从一个目标格消除一种颜色（最多不超过 limit 步）的方案
	MatchSet eval(int x, int y, int c, int limit) {
//cerr << "eval: " << x << ", " << y << " | " << c << endl; 
		MatchSet ms;
		ms.p.x = x;
		ms.p.y = y;
		ms.c = c;
		memset(B, 0, sizeof(B));
		deque<pair<int, Point> > Q;
		int unmatch_count = 4;
		for(int d = 0; d < 4; ++d) {
			Q.push_back(make_pair(0, Point(x+vx[d], y+vy[d], d)));
			B[x+vx[d]][y+vy[d]] = 1<<d;
		}
		while(unmatch_count && !Q.empty()) {
			int dis = Q.front().first;
			if(dis * unmatch_count + ms.dis >= limit) { 
				ms.dis = 99999999; 
				return ms; 
			}
//if(dis > 2) { ms.dis = 999999999; return dis; }
			Point p = Q.front().second;
//if(dis > dd[p.k]) { ++dist; ++dd[p.k]; }
//p.disp();
			Q.pop_front();
			// 这个点已经被匹配了的话，忽略
			if(ms.x[p.k] != -1) continue;
//cerr << int(G[p.x][p.y]) << p.k << endl;
			if(G[p.x][p.y] == c && !(B[p.x][p.y]&32)) {
//cerr<<"hit! " << dis << endl;
				ms.x[p.k] = p.x;
				ms.y[p.k] = p.y;
				ms.dis += dis;
				B[p.x][p.y] |= 32;
				--unmatch_count;
			}
			else {
//cerr<<"else!" << endl;
				for(int d = 0; d < 4; ++d) {
					p.x += dx[d];
					p.y += dy[d];
					if(p.inside(N) && (B[p.x][p.y]&(1<<p.k)) == 0) {
						B[p.x][p.y] |= (1<<p.k);
						Q.push_back(make_pair(dis+1, p));
					}
					p.x -= dx[d];
					p.y -= dy[d];
				}
			}
		}
		return ms;
	}

	void exec(MatchSet ms) {
		deque<int> q(0);
		Point s[4], t[4];
		for(int k = 0; k < 4; ++k) {
			t[k].x = ms.p.x + vx[k];
			t[k].y = ms.p.y + vy[k];
			s[k].x = ms.x[k];
			s[k].y = ms.y[k];
		}
		q.push_back(0);
		q.push_back(1);
		q.push_back(2);
		q.push_back(3);
		while(!q.empty()) {
			int k = q.front(), kk;
			q.pop_front();
			if(s[k] == t[k]) continue;
			while(s[k] != t[k]) {
				Point s2 = nx(s[k], t[k]);
				if(G[s2.x][s2.y] == G[s[k].x][s[k].y]) {
					for(kk = 0; kk < 4; ++kk) if(s2.x==s[kk].x && s2.y==s[kk].y) break;
					q.push_back(kk);
					swap(s[k], s[kk]);
//cerr<<ansv.size()/3<<endl;
				}
				else {
					if(mv(s[k], s2)) return;
					s[k] = s2;
				}
			}
		}
	}
	
	// Next
	Point nx(const Point& src, const Point& tar) {
		if(src.x < tar.x) return Point(src.x+1, src.y);
		if(src.x > tar.x) return Point(src.x-1, src.y);
		if(src.y < tar.y) return Point(src.x, src.y+1);
		if(src.y > tar.y) return Point(src.x, src.y-1);
		return Point(-1, -1);
	}

	// Move
	bool mv(Point& src, Point& tar) {
//cerr<<"move: "<<endl;
//src.disp();
//tar.disp();
		swap(G[src.x][src.y], G[tar.x][tar.y]);
		ansv.push_back(src.x);
		ansv.push_back(src.y);
		if(src.x < tar.x) ansv.push_back(2);
		else if(src.x > tar.x) ansv.push_back(0);
		else if(src.y < tar.y) ansv.push_back(1);
		else if(src.y > tar.y) ansv.push_back(3);

		bool ans = false;
		while (eliminate()) ans = true;
		return ans;

		set<pair<int, int> > st;
		for(int d = 0; d < 4; ++d) {
			st.insert(make_pair(src.x-vx[d], src.y-vy[d]));
			st.insert(make_pair(tar.x-vx[d], tar.y-vy[d]));
		}

		
		while(!st.empty()) {
			ans = ans || rm(st.begin()->first, st.begin()->second);
			st.erase(st.begin());
		}
		return ans;
	}

	// Fill
	void fl(const int& x, const int& y) {
		G[x][y] = get_tile();
		G[x][y+1] = get_tile();
		G[x+1][y] = get_tile();
		G[x+1][y+1] = get_tile();
//cerr<<"fill!!!!!"<<endl;
//for(int i = 0; i < N; ++i) {
//for(int j = 0; j < N; ++j) {
//cerr<<int(G[i][j]);
//}
//cerr<<endl;
//}
		rm(x-1, y-1);
		rm(x-1, y);
		rm(x-1, y+1);
		rm(x, y-1);
		rm(x, y);
		rm(x, y+1);
		rm(x+1, y-1);
		rm(x+1, y);
		rm(x+1, y+1);
	}

	// Remove
	bool rm(const int& x, const int& y) {
//cerr<<"remove: " <<x<<", " << y << endl;
		if(x >= 0 && x < N-1 && y >= 0 && y < N-1) {
			if(G[x][y] == G[x][y+1] &&
				G[x][y] == G[x+1][y] &&
				G[x][y] == G[x+1][y+1]) {
				SC += 1;
//cerr<<"remove ok!!"<<endl;
				fl(x, y);
				return true;
			}
		}
		return false;
	}

	bool eliminate() {

		for(int i = 1; i < N; ++i) {
			for(int j = 1; j < N; ++j) {
				if(G[i][j]==G[i-1][j] &&
						G[i-1][j-1]==G[i-1][j] &&
						G[i-1][j-1]==G[i][j-1] &&
						G[i][j]==G[i][j-1]) {
					++SC;
					G[i-1][j-1] = get_tile();
					G[i-1][j] = get_tile();
					G[i][j-1] = get_tile();
					G[i][j] = get_tile();
					return true;
				}
			}
		}
		return false;
	}

	
    
    vector <int> playIt(
            int colors,
            vector <string> board,
            int startSeed) {
//cerr << "seed = " << startSeed <<" | "<< startSeed%colors<<endl;
                
        // ****************************
        ansv.clear();
        N = board.size();
        C = colors;
		SD = startSeed;
        SC = 0;

		for(int i = (N-1)*(N-1)-1; i >= 0; --i) SEQ[i] = i;

		for(int i = 0; i < N; ++i)
			for(int j = 0; j < N; ++j)
				G[i][j] = board[i][j] - '0';

int lp=100000;
		for(int i = 0; i < N-1; ++i)
			for(int j = 0; j < N-1; ++j)
				rm(i, j);

		while(ansv.size() < 30000 && lp--) {

//cerr<<lp<<endl;
//cerr<<"LOOP!" << endl;
			MatchSet ms;
			ms.dis = 99999999;
			random_shuffle(SEQ, SEQ+(N-1)*(N-1));
			// k 是随机的次数
			for(int k = min(150, (N-1)*(N-1)), sx, sy; ms.dis > 1 && k--;) {
				sx = SEQ[k] / (N-1);
				sy = SEQ[k] % (N-1);
				char cc[6] = {};
				for(int d = 0, c; d < 4; ++d) {
					// 起码要有两个方格同色才干
					if(++cc[c = G[sx+vx[d]][sy+vy[d]]] == 2) {
						MatchSet ev = eval(sx, sy, c, ms.dis);
//if(ev.dis<1000)ev.disp();
						if(ev.dis < ms.dis) ms = ev;
					}
				}
			}
			// assert
			if(ms.dis == 99999999) {
				cerr << "没有找到任何方案！" << endl;
			}
//ms.disp();
			exec(ms);
			//return ansv;

//for(int i = 0; i < N; ++i) {
//for(int j = 0; j < N; ++j) {
//cerr<<int(G[i][j]);
//}
//cerr<<endl;
//}
		}
			while(ansv.size() < 30000) {
				ansv.push_back(1);
			}

        // ****************************
		ansv.resize(30000);
        return ansv;
    }
};


#ifdef _DEBUG

#include <iostream>
int main() {

    int colors, N, startSeed;
    cin >> colors >> N;
    vector<string> board(N);
    for (int i=0; i < N; i++) cin >> board[i];
    cin >> startSeed;
    vector<int> ret = SquareRemover().playIt(
        colors, board, startSeed);
    for (int i=0; i < 30000; i++)
		//cout << 1 << endl;
        cout << ret[i] << endl;
    return 0;
}

#endif 