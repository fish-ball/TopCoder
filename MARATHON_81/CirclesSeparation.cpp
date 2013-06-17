#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

const double EPS = 1e-9;

class Circle {
    
    int idx;
    double x, y, r, m;
    
    Circle(int i, double x, double y, double r, double m) :
        idx(i), x(x), y(y), r(r), m(m) {}

};

inline double dist2(const Circle& a, const Circle& b) {
    return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

inline bool overlap(const Circle& a, const Circle& b) {
    return dist2(a, b) < (a.r+b.r)*(a.r+b.r);
}

inline bool contain(const Circle& a, const Circle& b) {
    return dist2(a, b) < (a.r-b.r)*(a.r-b.r);
}

inline bool cross(const Circle& a, const Circle& b) {
    return dist2(a, b) < (a.r+b.r)*(a.r+b.r)
        && dist2(a, b) > (a.r-b.r)*(a.r-b.r);
}

class CirclesSeparation {
    
    int N;
    
    vector<Circle> C;
    
public:
    
    vector<double> minimumWork(
            vector <double> x,
            vector <double> y,
            vector <double> r,
            vector <double> m) {
        
        N = x.size();
        
        #if DEBUG 
        cerr << "N = " << N <<endl;
        #endif
        
        C.clear();
        
        for(int i = 0; i < N; ++i) {
            C.push_back(Circle(i, x[i], y[i], r[i], m[i]));
        }
        
        sort(C.rbegin(), C.rend(), wcCmp);
        
        for(int i = 0, j, k; i < N; ++i) {
            cerr << "i = " << i << endl;
            // 1. check if original point valid.
            if(__isValid(C[i], i)) { 
//cerr<<"1 !!!!!!!!!!!!!!!!!!!!!!" << endl;
//            cerr << C[i].c << ' ' << C[i].r << " " << C[i].idx << endl;
            continue; }
            // iteration
            double mind = 100, d;
            Point t(-1,-1), z = C[i].c;
            // 2. check if tangent to one circle.
            for(j = 0; j < i; ++j) {
                d = dist(C[j].c,z)-(C[j].r+C[i].r);
                if(d >= mind) continue;
                Point p = C[j].c - z;
                p = p / p.length();
                C[i].c = z + p * d;
//cerr << C[i].c << endl;
//cerr << dist(C[i].c, C[j].c)-C[i].r-C[j].r << endl;
                if(__isValid(C[i], i)) {
//cerr<<"2 !!!!!!!!!!!!!!!!!!!!!!" << endl;
                    t = C[i].c;
                    mind = d;
                }
            }
            C[i].c = t;
            //cerr << C[i].c << ' ' << C[i].r << " " << C[i].idx << endl;
            // 3. check if tangent to two circles.
            //if(false)
            for(j = 0; j < i; ++j) {
                for(k = j+1; k < i; ++k) {
                    Circle a = C[j], b = C[k];
                    a.r += C[i].r + 10*EPS;
                    b.r += C[i].r + 10*EPS;
                    vector<Point> vp = intersect(a, b);
                    if(vp.size() < 2) continue;
                    double d1 = dist(vp[0],z), d2 = dist(vp[1],z);
                    if(d1 < mind && __isValid(Circle(vp[0],C[i].r), i)) {
//cerr<<"3 !!!!!!!!!!!!!!!!!!!!!!" << endl;
//cerr << p << "!!!!!!!!!!!!" << endl;
                        t = vp[0];
                        mind = d1;
                    }
                    if(d2 < mind && __isValid(Circle(vp[1],C[i].r), i)) {
//cerr<<"3 !!!!!!!!!!!!!!!!!!!!!!" << endl;
//cerr << p << "!!!!!!!!!!!!" << endl;
                        t = vp[1];
                        mind = d2;
                    }
                }
            }
            C[i].c = t;
//            cerr << C[i].c << ' ' << C[i].r << " " << C[i].idx << endl;
        }
        return __getResult();
    }

};

#if DEBUG

#include <iostream>

int main() {
    int n;
    cin >> n;
    vector<double> x(n), y(n), r(n), m(n), ret;
    for(int i = 0; i < n; ++i) cin >> x[i];
    for(int i = 0; i < n; ++i) cin >> y[i];
    for(int i = 0; i < n; ++i) cin >> r[i];
    for(int i = 0; i < n; ++i) cin >> m[i];
    CirclesSeparation cs;
    ret = cs.minimumWork(x, y, r, m);
    for(int i = 0; i < ret.size(); ++i) {
        cout << ret[i] << endl;
    }
    system("pause");
    return 0;
}

#endif
