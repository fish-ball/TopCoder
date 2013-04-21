#define DEBUG 0


//////////////////////////////////////////////////////////////////////////
//                     Geometric class, Version 2.3                     //
//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  V2.0(2008-9-16):                                                    // 
//      -Rewrite all basic component from V1.12;                        //
//  V2.1(2009-3-21):                                                    //
//      *Method: perpendicular(Point, Line)                             //
//      *Method: angelBisector(Point, Point, Point)                     //
//      -Functions to archieve various triangle centers added:          //
//      *Method: gravityCenter(Point, Point, Point)                     //
//      *Method: outterCenter(Point, Point, Point)                      //
//      *Method: innerCenter(Point, Point, Point)                       //
//      *Method: orthoCenter(Point, Point, Point)                       //
//  V2.2(2009-5-31):                                                    //
//      *Method: Polygon::perimeter() const                             //
//      *Method: parallel(Line, Line)                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//                   Powered by ELF, Copyright(c) 2012                  //
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

#ifndef GEOMETRIC20_XPER
#define GEOMETRIC20_XPER

using namespace std;

//////////////////////////////////////////////////////////////////////////
//             Constants and precision comaprision operator.            //
//////////////////////////////////////////////////////////////////////////

const double PI = acos(-1.0);
const double EPS = 1e-10;


#ifndef _MSC_VER
double hypot(double lhs, double rhs) { return sqrt(lhs*lhs+rhs*rhs); }
#endif // _MSC_VER

struct Point;
struct Segment;
struct Line;
struct Circle;
struct Polygon;
typedef Point Vector;
typedef Segment Rectangle;


inline bool lt(double lhs, double rhs) { return rhs - lhs >= EPS; }
inline bool le(double lhs, double rhs) { return rhs - lhs >= -EPS; }
inline bool gt(double lhs, double rhs) { return lhs - rhs >= EPS; }
inline bool ge(double lhs, double rhs) { return lhs - rhs >= -EPS; }
inline bool eq(double lhs, double rhs) { return fabs(rhs-lhs) < EPS; }
inline bool ne(double lhs, double rhs) { return fabs(rhs-lhs) >= EPS; }

inline bool zr(double lhs) { return fabs(lhs) < EPS; }
inline bool nz(double lhs) { return fabs(lhs) >= EPS; }
inline bool ps(double lhs) { return lhs >= EPS; }
inline bool nn(double lhs) { return lhs >= -EPS; }
inline bool ng(double lhs) { return lhs < -EPS; }
inline bool np(double lhs) { return lhs < EPS; }

// Distance
double dist(const Point&, const Point&);
double dist(const Point&, const Line&);
double dist(const Line&, const Point&);
double dist(const Segment&, const Point&);
double dist(const Point&, const Segment&);
double dist(const Line&, const Segment&);
double dist(const Segment&, const Line&);

// Relations
bool parallel(const Line&, const Line&);
bool cross(const Line&, const Segment&);
bool cross(const Segment&, const Line&);
bool cross(const Line&, const Circle&);
bool cross(const Circle&, const Line&);
bool cross(const Circle&, const Circle&);
bool onSegment(const Point&, const Segment&);
bool cross(const Segment&, const Segment&);
const Point intersect(const Line&, const Line&);
const vector<Point> intersect(const Line&, const Circle&);
const vector<Point> intersect(const Circle&, const Line&);
const Point pedal(const Line&, const Point&);
const Point pedal(const Point&, const Line&);
const Point mirror(const Line&, const Point&);
const Point mirror(const Point&, const Line&);
const Line perpendicular(const Point&, const Line&);
const Line bisector(const Point&, const Point&);
// Angle bisector of angle CAB
const Line angleBisector(const Point&, const Point&, const Point&);
const Circle circumcircle(const Point&, const Point&, const Point&);

//////////////////////////////////////////////////////////////////////////
//                        Point struct definition.                      //
//////////////////////////////////////////////////////////////////////////

struct Point {
    double x, y;
    Point(double x=0, double y=0) : x(x), y(y) {}
    void get() { scanf("%lf%lf", &x, &y); }
    void put() const { printf("(%lf, %lf)", x, y); }
    operator bool() const { return nz(x) && nz(y); }
    double length() const { return hypot(x, y); }
};

// Point struct stream operator.
istream& operator>>(istream& is, Point& p) 
    { return is >> p.x >> p.y;}
ostream& operator<<(ostream& os, const Point& p) 
    { return os << "(" << p.x << ", " << p.y << ")"; }

// Point struct comparision operator.
bool operator==(const Point& lhs, const Point& rhs)
    { return eq(lhs.x, rhs.x) && eq(lhs.y, rhs.y); }
bool operator!=(const Point& lhs, const Point& rhs)
    { return ne(lhs.x, rhs.x) && ne(lhs.y, rhs.y); }
bool operator<(const Point& lhs, const Point& rhs)
    { return lt(lhs.x, rhs.x) || eq(lhs.x, rhs.x) && lt(lhs.y, rhs.y); }
bool operator<=(const Point& lhs, const Point& rhs)
    { return lt(lhs.x, rhs.x) || eq(lhs.x, rhs.x) && le(lhs.y, rhs.y); }
bool operator>(const Point& lhs, const Point& rhs)
    { return gt(lhs.x, rhs.x) || eq(lhs.x, rhs.x) && gt(lhs.y, rhs.y); }
bool operator>=(const Point& lhs, const Point& rhs)
    { return gt(lhs.x, rhs.x) || eq(lhs.x, rhs.x) && ge(lhs.y, rhs.y); }

// Point struct scalar operator.
const Point operator+(const Point& lhs, double rhs)
    { return Point(lhs.x+rhs, lhs.y+rhs); }
const Point operator+(double lhs, const Point& rhs)
    { return Point(lhs+rhs.x, lhs+rhs.y); }
const Point operator-(const Point& lhs, double rhs)
    { return Point(lhs.x-rhs, lhs.y-rhs); }
const Point operator-(double lhs, const Point& rhs)
    { return Point(lhs-rhs.x, lhs-rhs.y); }
const Point operator*(double lhs, const Point& rhs)
    { return Point(lhs*rhs.x, lhs*rhs.y); }
const Point operator*(const Point& lhs, double rhs)
    { return Point(lhs.x*rhs, lhs.y*rhs); }
const Point operator/(const Point& lhs, double rhs)
    { return Point(lhs.x/rhs, lhs.y/rhs); }
Point& operator+=(Point& lhs, double rhs)
    { lhs.x += rhs; lhs.y += rhs; return lhs; }
Point& operator-=(Point& lhs, double rhs)
    { lhs.x -= rhs; lhs.y -= rhs; return lhs; }
Point& operator*=(Point& lhs, double rhs)
    { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
Point& operator/=(Point& lhs, double rhs)
    { lhs.x /= rhs; lhs.y /= rhs; return lhs; }

// Point struct vector operator.
const Point operator+(const Point& lhs, const Point& rhs)
    { return Point(lhs.x+rhs.x, lhs.y+rhs.y); }
const Point operator-(const Point& lhs, const Point& rhs)
    { return Point(lhs.x-rhs.x, lhs.y-rhs.y); }
const Point operator~(const Point& lhs) 
    { return Point(-lhs.y, lhs.x); }
double operator^(const Point& lhs, const Point& rhs)
    { return lhs.x*rhs.y - lhs.y*rhs.x; }
double operator*(const Point& lhs, const Point& rhs)
    { return lhs.x*rhs.x + lhs.y*rhs.y; }
Point& operator+=(Point& lhs, const Point& rhs)
    { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
Point& operator-=(Point& lhs, const Point& rhs)
    { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }

//////////////////////////////////////////////////////////////////////////
//                       Segment struct definition.                     //
//////////////////////////////////////////////////////////////////////////

struct Segment {
    Point a, b;
    Segment() : a(), b() {}
    Segment(Point a, Point b) : a(a), b(b) {}
    double length() const { return (b-a).length(); }
};

//////////////////////////////////////////////////////////////////////////
//                         Line struct definition.                      //
//////////////////////////////////////////////////////////////////////////

struct Line {
    double A, B, C;
    Line() : A(0), B(0), C(0) {}
    Line(double A, double B, double C) : A(A), B(B), C(C) {}
    Line(const Point& lhs, const Point& rhs) :
        A(rhs.y - lhs.y), B(lhs.x - rhs.x), C(-(lhs.x*A+lhs.y*B)) {}
    Line(double slope, const Point& p) :
        A(slope), B(-1.0), C(p.y-slope*p.x ) {}
    Line(const Point& p, double slope) :
        A(slope), B(-1.0), C(p.y-slope*p.x ) {}
    Line(const Segment& s) { *this = Line(s.a, s.b); }
    double eval(const Point& p) const { return A*p.x+B*p.y+C; }
};

//////////////////////////////////////////////////////////////////////////
//                        Circle struct definition.                     //
//////////////////////////////////////////////////////////////////////////

struct Circle {
    Point c;
    double r;
    Circle() {}
    Circle(Point c, double r) : c(c), r(r) {}
    Circle(double r, Point c) : c(c), r(r) {}
}; 

//////////////////////////////////////////////////////////////////////////
//                       Polygon struct definition.                     //
//////////////////////////////////////////////////////////////////////////

struct Polygon : public vector<Point> {
    Polygon() : vector<Point>(0) {}
    Polygon(const Polygon& P) : vector<Point>(P) {}
    Polygon(const vector<Point>& V) : vector<Point>(V) {}
    template<class Iter> Polygon(Iter begin, Iter end) :
        vector<Point>(begin, end) {}
    double area() const;
    double perimeter() const;
    const Point center() const;
    int within(const Point& p) const;
};

//////////////////////////////////////////////////////////////////////////
//                       Relations between objects.                     //
//////////////////////////////////////////////////////////////////////////

bool parallel(const Line& a, const Line& b)
    { return eq(a.A * b.B, a.B * b.A); }
bool cross(const Line& l, const Segment& s) {
    double a = l.eval(s.a), b = l.eval(s.b);
    return zr(a) || zr(b) || ps(a)^ps(b);
}
bool cross(const Segment& s, const Line& l)
    { return cross(l, s); }
bool cross(const Line& l, const Circle& c) 
    { return le(dist(c.c, l), c.r);  }
bool cross(const Circle& c, const Line& l) 
    { return le(dist(c.c, l), c.r);  }
bool cross(const Circle& c1, const Circle& c2) {
    double d = dist(c1.c, c2.c);
    return le(d, c1.r+c2.r) && ge(d, fabs(c1.r-c2.r));
}
bool onSegment(const Point& p, const Segment& s)
    { return zr((p-s.a)^(p-s.b)) && np((p-s.a)*(p-s.b)); }
bool cross(const Segment& v, const Segment& w) {
    if(onSegment(v.a, w) ||
        onSegment(v.b, w) ||
        onSegment(w.a, v) ||
        onSegment(w.b, v))
        return true;
    return ( ps((v.a-w.a)^(w.b-w.a)) ^ ps((v.b-w.a)^(w.b-w.a)) )
        && ( ps((w.a-v.a)^(v.b-v.a)) ^ ps((w.b-v.a)^(v.b-v.a)) );
}
const Point intersect(const Line& a, const Line& b) {
    return Point( (a.B*b.C-b.B*a.C)/(b.B*a.A-a.B*b.A),
                  (a.A*b.C-b.A*a.C)/(b.A*a.B-a.A*b.B) );
}
const vector<Point> intersect(const Line& l, const Circle& c) {
    vector<Point> ans;
    if(cross(l, c)) {
        Point p = pedal(c.c, l);
        double d = dist(p, c.c);
        double len = sqrt(fabs(c.r*c.r-d*d));
        Vector a(-l.B, l.A);
        if(zr(len)) ans.push_back(p);
        else {
            ans.push_back(p+a*len/a.length());
            ans.push_back(p-a*len/a.length());
        }
    }
    return ans;   
}
const vector<Point> intersect(const Circle& c, const Line& l)
    { return intersect(l, c); }
const vector<Point> intersect(const Circle& c1, const Circle& c2) {
    if(cross(c1, c2)) {
        double d = dist(c1.c, c2.c);
        double t = ((c1.r*c1.r-c2.r*c2.r)/d/d+1)*0.5;
        Point o = c1.c+(c2.c-c1.c)*t;
        return intersect(c1, Line(o, o+~(c1.c-c2.c)));
    }
    else return vector<Point>(0);
}
const Point pedal(const Line& l, const Point& p ) {
    double k = l.eval(p)/(l.A*l.A+l.B*l.B);
    return Point(p.x-l.A*k, p.y-l.B*k);
}
const Point pedal(const Point& p, const Line& l)
    { return pedal(l, p); }
const Point mirror(const Line& l, const Point& p)
    { return pedal(l, p) * 2.0 - p; }
const Point mirror(const Point& p, const Line& l)
    { return pedal(l, p) * 2.0 - p; }
const Line perpendicular(const Point& p, const Line& l)
    { return Line(p, pedal(p, l)); }
const Line bisector(const Point& a, const Point& b)
    { return Line((a+b)/2.0, (a+b)/2.0+~(a-b)); }
// Angle bisector of angle CAB
const Line angleBisector(const Point& A, const Point& B, const Point& C) {
    Point v = (B - A) / (B - A).length();
    Point w = (C - A) / (C - A).length();
    Point O = (A + v + A + w) / 2.0;
    return Line(A, O);
}
const Circle circumcircle(const Point& a, const Point& b, const Point& c) {
    Point p = intersect(bisector(a, b), bisector(b, c));
    return Circle(p, dist(a, p));
}

//////////////////////////////////////////////////////////////////////////
//                       Distances between objects.                     //
//////////////////////////////////////////////////////////////////////////

double dist(const Point& lhs, const Point& rhs)
    { return (lhs-rhs).length(); }
double dist(const Point& p, const Line& l)
    { return fabs(l.eval(p))/hypot(l.A, l.B); }
double dist(const Line& l, const Point& p)
    { return fabs(l.eval(p))/hypot(l.A, l.B); }
double dist(const Segment& s, const Point& p) {
    if(ps((s.a-p)*(s.a-s.b))^ps((s.b-p)*(s.b-s.a))) 
        return min(dist(s.a, p), dist(s.b, p));
    return dist(p, Line(s.a, s.b));
}
double dist(const Point& p, const Segment& s)
    { return dist(s, p); } 
double dist(const Line& l, const Segment& s) {
    if(cross(l, s)) return 0.0;
    return min(dist(s.a, l), dist(s.b, l));
}
double dist(const Segment& s, const Line& l)
    { return dist(l, s); }

//////////////////////////////////////////////////////////////////////////
//                       Basic triangle operation.                      //
//////////////////////////////////////////////////////////////////////////

//   We assume that the three vertex of the triangle is pA-pB-pC,
// while the angle assign to them are A-B-C, and the opposite side
// of them are a-b-c, the funtions below follows this rule of signs.

// Area of triagles calculation functions.
double areaSSA(double a, double b, double C)
    { return 0.5*a*b*sin(C); }
double areaSSS(double a, double b, double c)
    { double s=(a+b+c)*0.5; return sqrt(s*(s-a)*(s-b)*(s-c)); }
double areaSAA(double a, double B, double C)
    { return a*a*0.5/(1.0/tan(B)+1.0/tan(C)); }
double area(const Point& pA, const Point& pB, const Point& pC)
    { return fabs((pB-pA)^(pC-pA))*0.5; }
double area(const Vector& v, const Vector& w)
    { return fabs(v^w)*0.5; }
double side_c(double a, double b, double C = PI/2.0) {
    double ans = a*a + b*b - 2.0*a*b*cos(C);
    return nn(ans) ? sqrt(fabs(ans)) : -1.0;
}
const Point gravityCenter(const Point& A, const Point& B, const Point& C)
    { return (A + B + C) / 3.0; }
const Point outterCenter(const Point& A, const Point& B, const Point& C)
    { return intersect(bisector(A, B), bisector(A, C)); }
const Point innerCenter(const Point& A, const Point& B, const Point& C)
    { return intersect(angleBisector(A, B, C), angleBisector(C, B, A)); }
const Point orthoCenter(const Point& A, const Point& B, const Point& C)
    { return intersect(perpendicular(A, Line(B, C)), perpendicular(B, Line(A, C))); }

//////////////////////////////////////////////////////////////////////////
//                      Polygon Method: Convex Hull                     //
//////////////////////////////////////////////////////////////////////////
double Polygon::area() const {
    double ans = 0.0;
    for(size_t i=0; i<size(); ++i)
        ans += (at(i) ^ at((i+1)%size()));
    return fabs(ans)*0.5;
}

double Polygon::perimeter() const {
    double ans = 0.0;
    for(size_t i=0; i<size(); ++i)
        ans += dist(at(i), at((i+1)%size()));
    return ans;
}

const Point Polygon::center() const {
    Point ans(0, 0), p;
    double W = 0.0, w;
    for(size_t i=0; i<size(); ++i) {
        W += (w = (at(i) ^ at((i+1)%size())));
        p = (at(i) + at((i+1)%size())) / 3.0 * w;
        ans += p;
    }
    return ans / W;
}

int quadrant(const Point& p) {
    if(zr(p.x) && zr(p.y))  return -1;
    if(p.x > 0 && p.y >= 0) return 0;
    if(p.x <= 0 && p.y > 0) return 1;
    if(p.x < 0 && p.y <= 0) return 2;
    if(p.x >= 0 && p.y < 0) return 3;
	return -1;
}

// onside returns -1, while within returns 1, outside returns 0
int Polygon::within(const Point& p) const {
    int n = 0, qa, qb;
    Point a, b = back() - p;
    for(size_t i=0; i<size(); ++i) {
        qa = quadrant(a=b);
        qb = quadrant(b=at(i)-p);
        if(qa==-1 || qb==-1) return -1;
        else if(qa==qb) continue;
        else if(qa==(qb+3)%4) ++n;
        else if(qa==(qb+1)%4) --n;
        else if(zr(a^b)) return -1;
        else n += ps(a^b) ? 2 : -2;
    }
    return n ? 1 : 0;
}

Point refP, ctrP;
double xmult(Point lhs, Point rhs, Point ref) {
    return (lhs - ref) ^ (rhs - ref);
}
    
bool acmp(const Point& lhs, const Point& rhs) {
    return  ng(xmult(lhs, rhs, refP)) ||
            zr(xmult(lhs, rhs, refP)) &&
            ng(xmult(lhs, rhs, ctrP));
}
const Polygon ConvexHull(Polygon P) {
    size_t i, k = 0;
    for(refP = ctrP = P[0], i = 1; i < P.size(); ++i ) {
        ctrP = ctrP + P[i];
        if(gt(refP.y, P[i].y) || eq(refP.y, P[i].y) &&
            gt(refP.x, P[i].x))  refP = P[k=i];
    }
    ctrP.x /= P.size();
    ctrP.y /= P.size();
    swap(P[k], P[0]);
    sort(P.begin() + 1, P.end(), acmp);
    if( P.size() < 3 )  return  P;
    Polygon CH(P.begin(), P.begin() + 3);
    for(i = k = 3; i < P.size(); ++i, ++k) {
        while(k > 2 && np(xmult(CH[k-2], P[i], CH[k-1])))
            {CH.pop_back(); k--;}
        CH.push_back( P[i] );
    }
    return CH;
}

#endif // GEOMETRIC20_XPER

//////////////////////////////////////////////////////////////////////////
//                     Geometric class, Version 2.3                     //
//                   Powered by ELF, Copyright(c) 2012                  //
//////////////////////////////////////////////////////////////////////////

bool operator < (const Line& a, const Line& b) { return a.A < b.A; }
bool operator < (const Segment& a, const Segment& b) 
    { return a.length() < b.length(); }

#include <cstdio>   
#include <cstdlib> 
#include <cmath>

#ifndef BMP_XPER

#define BYTE unsigned char   
#define WORD unsigned short int   
#define DWORD unsigned int   
#define SDWORD signed int   
#define SBYTE signed char   
#define SWORD signed short int   

  
struct BM_header {   
    WORD BMP_id ; // 'B''M'   
    DWORD size; // size in bytes of the BMP file   
    DWORD zero_res; // 0   
    DWORD offbits; // 54   
    DWORD biSize; // 0x28   
    DWORD Width;  // X   
    DWORD Height;  // Y   
    WORD  biPlanes; // 1   
    WORD  biBitCount ; // 24   
    DWORD biCompression; // 0 = BI_RGB   
    DWORD biSizeImage; // 0   
    DWORD biXPelsPerMeter; // 0xB40   
    DWORD biYPelsPerMeter; // 0xB40   
    DWORD biClrUsed; //0   
    DWORD biClrImportant; //0   
};

struct s_RGB { 
    BYTE BLUE, GREEN, RED;
    s_RGB() {}
    s_RGB(BYTE r, BYTE g, BYTE b) 
        : RED(r), GREEN(g), BLUE(b) {}
};

// 返回给定的 rgb 色彩的色相角。
int hue(int r, int g, int b) {
	if(r==g && g==b) return -1;
	if(r>=g && g>=b) return       60*(g-b)/(r-b);
	if(g> r && r>=b) return 120 - 60*(r-b)/(g-b);
	if(g>=b && b> r) return 120 + 60*(b-r)/(g-r);
	if(b> g && g>=r) return 240 - 60*(g-r)/(b-r);
	if(b> r && r>=g) return 240 + 60*(r-g)/(b-g);
	if(r>=b && b> g) return 360 - 60*(b-g)/(r-g);
	return -1;
}

#endif //BMP_XPER



char sbuf[20];

string st[] = {
    "DOWN", "DOWN", "OFF", "CENTER", "OFF",
    "OFF", "OFF", "OFF", "OFF", "OFF",
    "OFF", "OFF", "OFF", "OFF", "OFF",
    "DOWN", "OFF", "CENTER", "OFF", "OFF",
    "UP", "ON"
};

double PS[22][2] = {
    {	0.659798903	,	0.156708861	},
    {	0.659798903	,	0.156708861	},
    {	0.756691042	,	0.156708861	},
    {	0.757351852	,	0.478202532	},
    {	0.759648799	,	0.398455696	},
    {	0.758719852	,	0.561746835	},
    {	0.182920518	,	0.396200762	},
    {	0.304879852	,	0.39652725	},
    {	0.4287061	,	0.395196451	},
    {	0.185185185	,	0.480088945	},
    {	0.307333333	,	0.481444867	},
    {	0.429537037	,	0.480114068	},
    {	0.185528757	,	0.563977128	},
    {	0.305955473	,	0.563865653	},
    {	0.426716141	,	0.563776933	},
    {	0.20287594	,	0.787944162	},
    {	0.205861423	,	0.706763959	},
    {	0.497969925	,	0.787113924	},
    {	0.497977528	,	0.707367089	},
    {	0.496254682	,	0.862025316	},
    {	0.790093633	,	0.788341772	},
    {	0.790093633	,	0.706075949	}
};

int X1[22], Y1[22], X2[22], Y2[22];
string S[22];

vector<Point> vv;

double diff;
string folder = "";

/* Image class
Axis direction (W X H):
   01234567 W
   x------> 
0 y........
1 |........
2 |........
3 |........
4 v........
H

*/


#include <deque>
using namespace std;

class Image {

public:
    
    int W;
    int H;
    BYTE* _R;
    BYTE* _G;
    BYTE* _B;
    
    inline BYTE& r(int x, int y) { return _R[y * W + x]; }
    inline BYTE& g(int x, int y) { return _G[y * W + x]; }
    inline BYTE& b(int x, int y) { return _B[y * W + x]; }

	// 构造一个图像
	// w 宽度
	// h 高度
	// c 是否为彩色
    Image(int w, int h, bool c = false) : W(w), H(h) {
        
        _R = new BYTE[W*H]; memset(_R, 0, W*H);
        
        if(!c) { _G = _B = _R; }
        else {
            _G = new BYTE[W*H]; memset(_G, 0, W*H);
            _B = new BYTE[W*H]; memset(_B, 0, W*H);
        }
    }

    Image(const vector<int> &img) {
        H = img[0];
        W = img[1];
        
        _R = new BYTE[W*H];
        _G = new BYTE[W*H];
        _B = new BYTE[W*H];
        
        int x, y, k = 2, p;
        for(y = 0; y < H; ++y) {
            for(x = 0; x < W; ++x, ++k) {
                p = img[k];
                b(x,y) = p & 0xFF; p >>= 8;
                g(x,y) = p & 0xFF; p >>= 8;
                r(x,y) = p & 0xFF;
            }
        }
    }

    ~Image() {
        if(_G != _R) delete [] _G;
        if(_B != _R) delete [] _B;
        delete [] _R;
    }
    
	// 判断一个点坐标是否在图像范围内
    bool inside(int x, int y) { return x>=0 && x<W && y>=0 && y<H; }
    bool inside(const Point &p) { return p.x>=0 && p.x<W && p.y>=0 && p.y<H; }
    
	// 输出图像到文件
    void draw(string fname = "image.bmp") {
        
        FILE *fp_bitmap=fopen(fname.c_str(),"wb");

        SWORD x,y;
        BYTE pixel=1;
        BM_header BH;
        BYTE zero_byte=0;
        
        if (fp_bitmap==NULL) return;
        BH.BMP_id = 'M'*256+'B';     fwrite(&BH.BMP_id,2,1,fp_bitmap);
        BH.size = 54+H*(W*3);
        fwrite(&BH.size,4,1,fp_bitmap);
        BH.zero_res = 0;             fwrite(&BH.zero_res,4,1,fp_bitmap);
        BH.offbits = 54;             fwrite(&BH.offbits,4,1,fp_bitmap);
        BH.biSize = 0x28;            fwrite(&BH.biSize,4,1,fp_bitmap);
        BH.Width = W;                fwrite(&BH.Width,4,1,fp_bitmap);
        BH.Height = H;               fwrite(&BH.Height,4,1,fp_bitmap);
        BH.biPlanes = 1;             fwrite(&BH.biPlanes,2,1,fp_bitmap);
        BH.biBitCount = 24;          fwrite(&BH.biBitCount,2,1,fp_bitmap);
        BH.biCompression = 0;        fwrite(&BH.biCompression,4,1,fp_bitmap);
        BH.biSizeImage = 0;          fwrite(&BH.biSizeImage,4,1,fp_bitmap);
        BH.biXPelsPerMeter = 0xB40;  fwrite(&BH.biXPelsPerMeter,4,1,fp_bitmap);
        BH.biYPelsPerMeter = 0xB40;  fwrite(&BH.biYPelsPerMeter,4,1,fp_bitmap);
        BH.biClrUsed = 0;            fwrite(&BH.biClrUsed,4,1,fp_bitmap);
        BH.biClrImportant = 0;       fwrite(&BH.biClrImportant,4,1,fp_bitmap);
        
        s_RGB pix;
        
        for(y = H-1; y >= 0; --y) {
            for(x = 0; x < W; ++x) {
                pix = s_RGB(r(x,y),g(x,y),b(x,y));
                fwrite(&pix, 3, 1, fp_bitmap);
            }
        }
        
        fclose(fp_bitmap);
    }


    void shrink() {
        int i, j;
        int W2 = W/2, H2 = H/2;
        BYTE* rr = new BYTE[W2*H2], *gg, *bb;
        if(_R != _G) {
            gg = new BYTE[W2*H2];
            bb = new BYTE[W2*H2];
        }
        for(i = 0; i < W2; ++i) {
            for(j = 0; j < H2; ++j) {
                rr[i+j*W2] = (0 + 
                    r((i<<1),(j<<1)) +
                    r((i<<1)+1,(j<<1)) +
                    r((i<<1),(j<<1)+1) +
                    r((i<<1)+1,(j<<1)+1)) >> 2;
                if(_R!=_G) {
                    gg[i+j*W2] = (0 + 
                        g((i<<1),(j<<1)) +
                        g((i<<1)+1,(j<<1)) +
                        g((i<<1),(j<<1)+1) +
                        g((i<<1)+1,(j<<1)+1)) >> 2;
                    bb[i+j*W2] = (0 + 
                        b((i<<1),(j<<1)) +
                        b((i<<1)+1,(j<<1)) +
                        b((i<<1),(j<<1)+1) +
                        b((i<<1)+1,(j<<1)+1)) >> 2;
                }
            }
        }
        W /= 2;
        H /= 2;
        if(_R != _G) {
            delete [] _G;
            delete [] _B;
        }
        delete [] _R;
        _R = rr;
        _G = gg;
        _B = bb;
    }

	// 输出高斯滤波后的图片
    void getFilted() {
        
        Image e(W, H, true);
        
        int gauss[][5] = {
            { 2,  4,  5,  4,  2},
            { 4,  9, 12,  9,  4},
            { 5, 12, 15, 12,  5},
            { 4,  9, 12,  9,  4},
            { 2,  4,  5,  4,  2}
        };

        int x, y, i, j, sr, sg, sb, acc;
        
    	for(x = 0; x < W; x++){
    		for(y = 0; y < H; y++){
                sr = sg = sb = acc = 0;
                for(i = max(2-x, 0); i < min(2+W-x, 5); ++i) {
                    for(j = max(2-y, 0); j < min(2+H-y, 5); ++j) {
                        sr += gauss[i][j] * r(x-2+i, y-2+j);
                        if(_R!=_G) {
                            sg += gauss[i][j] * g(x-2+i, y-2+j);
                            sb += gauss[i][j] * b(x-2+i, y-2+j);
                        }
                        acc += gauss[i][j];
                    }
                }
                e.r(x, y) = sr / acc;
                if(_R!=_G) {
                    e.g(x, y) = sg / acc;
                    e.b(x, y) = sb / acc;
                }
    		}
    	}
        
        memcpy(_R, e._R, W*H);
        if(_R!=_G) {
            memcpy(_G, e._G, W*H);
            memcpy(_B, e._B, W*H);
        }
    }
    
    
    // 输出灰度化的图像 
    Image getGray() {
        
        Image gray(W, H, false);
        
        int i, j;
        
        for(i = 0; i < W; ++i) {
            for(j = 0; j < H; ++j) {
                gray.r(i,j) = (BYTE)(
                    0.072169 * b(i,j) +
                    0.715160 * g(i,j) +
                    0.212671 * r(i,j)
                );
            }
        }
        
        return gray;
    }
    
    // 验证在给定的边缘图中得出的直线的最大连通长度 
    Segment getSegment(const Line& line
            , int x1 = 0, int x2 = -1
            , int y1 = 0, int y2 = -1) {

        if(x2 < x1) x2 = W;
        if(y2 < y1) y2 = H;
        int tolerance;
        
        if(folder == "Sim") {
            tolerance = H / 200;
        }
        else if(folder == "Lab") {
            tolerance = H / 200;
        }
        else if(folder == "Lab2") {
            tolerance = H / 100;
        }
        else if(folder == "ISS") {
            tolerance = H / 200;
        }
        else {
            tolerance = H / 200;
        }
        
        Segment ret;
        
        int mxlen = 0;
        
        // for x side check 
        if(fabs(line.A) < fabs(line.B)) {
            int sx = x1, tx = x1, px = x1;
            for(int x = x1, y; x < x2; ++x) {
                y = int(intersect(line, Line(1,0,-x)).y);
                if(inside(x,y) && r(x,y) == 255 ||
                        inside(x,y+1) && r(x,y+1) == 255 ||
                        inside(x,y-1) && r(x,y-1) == 255 ||
                        inside(x,y+2) && r(x,y+2) == 255 ||
                        inside(x,y-2) && r(x,y-2) == 255 ||
                        inside(x,y+3) && r(x,y+3) == 255 ||
                        inside(x,y-3) && r(x,y-3) == 255) {
                    tx = max(tx, x);
                    // 仅当断口超过容差并且超过当前最长段的 1/8 时才打断线段 
                    if(tx - px > tolerance && (tx-px)*8 > mxlen ) sx = tx;
                    px = tx;
                    if(tx-sx > mxlen) {
                        mxlen = tx-sx;
                        ret = Segment(
                            intersect(line, Line(1, 0, -sx)),
                            intersect(line, Line(1, 0, -tx))
                            );
                    }
                }
            }
        }
        
        // for y side check
        else {
            int sy = y1, ty = y1, py = y1;
            for(int y = y1, x; y < y2; ++y) {
                x = int(intersect(line, Line(0,1,-y)).x);
                if(inside(x,y) && r(x,y) == 255 ||
                        inside(x+1,y) && r(x+1,y) == 255 ||
                        inside(x-1,y) && r(x-1,y) == 255 ||
                        inside(x+2,y) && r(x+2,y) == 255 ||
                        inside(x-2,y) && r(x-2,y) == 255 ||
                        inside(x+3,y) && r(x+3,y) == 255 ||
                        inside(x-3,y) && r(x-3,y) == 255) {
                    ty = max(ty, y);
                    if(ty - py > tolerance && (ty-py)*8 > mxlen ) sy = ty;
                    py = ty;
                    if(ty-sy > mxlen) {
                        mxlen = ty-sy;
                        ret = Segment(
                            intersect(line, Line(0, 1, -sy)),
                            intersect(line, Line(0, 1, -ty))
                            );
                    }
                }
            }
        }

        return ret;
    }
    
    // 获取 canny 算法得到的边缘。
    Image getEdge() {
        
        int i, j, x, y, idx;
		int dx[8] = {1,1,0,-1,-1,-1,0,1};  
		int dy[8] = {0,1,1,1,0,-1,-1,-1};
        
        // 1. 灰度化图像 
        Image gr = getGray();
        
        if(DEBUG) {
            gr.draw("gray.bmp");
            cerr << "gray ready!" << endl;
        }

        // 2. 高斯滤波
        // 在输入图像之前做好滤波就可以了，边缘检测中不进行滤波。 
        // Image ft = gray.getFilted();
        

        // 3. 图像增强——计算图像梯度及其方向
        double* P = new double[W*H];
        double* Q = new double[W*H];
        int* M = new int[W*H];
        double* Theta = new double[W*H];

        // 计算x,y方向的偏导数  
        for(i = 0; i < (W-1); i++) {  
            for(j = 0; j < (H-1); j++) {
                idx = i+j*W;
                P[idx] = (0. + gr.r(i+1,j) - gr.r(i,j) + gr.r(i+1,j+1) - gr.r(i,j+1)) * 2;  
                Q[idx] = (0. + gr.r(i,j) - gr.r(i,j+1) + gr.r(i+1,j) - gr.r(i+1,j+1)) * 2;  
            }  
        }

        if(DEBUG) {
            cerr << "gradient ready!" << endl;
        }

        // 计算梯度幅值和梯度的方向  
        for(i = 0; i < W; i++) {
            for(j = 0; j < H; j++) {
                idx = i+j*W;
                M[idx] = (int)(sqrt(P[idx]*P[idx] + Q[idx]*Q[idx])+0.5);  
                Theta[idx] = atan2(Q[idx], P[idx]) * 180 / PI;
                if(Theta[idx] < 0)  
                    Theta[idx] += 360;              //将这个角度转换到0~360范围  
            }  
        }
        if(DEBUG) {
            cerr << "gradient amp and phase ready!" << endl;
        }
        
        // 4. 非极大值抑制 
        Image ls(W, H, false);
        int g1=0, g2=0, g3=0, g4=0;                            //用于进行插值，得到亚像素点坐标值  
        double dTmp1=0.0, dTmp2=0.0;                           //保存两个亚像素点插值得到的灰度数据  
        double dWeight=0.0;                                    //插值的权重 
        
        for(i=1; i<(W-1); i++) { 
            
            for(j=1; j<(H-1); j++) {
                
                //当前点在图像数组中的索引值  
                int idx = i+j*W;
                
                //如果当前梯度幅值为0，则不是局部最大对该点赋为0
                if(M[idx] == 0)  ls.r(i,j) = 0;         
                else {  
                    ////////首先判断属于那种情况，然后根据情况插值///////  
                    ////////////////////第一种情况///////////////////////  
                    /////////       g1  g2                  /////////////  
                    /////////           C                   /////////////  
                    /////////           g3  g4              /////////////  
                    /////////////////////////////////////////////////////  
                    if( ((Theta[idx]>=90)&&(Theta[idx]<135)) ||   
                            ((Theta[idx]>=270)&&(Theta[idx]<315))) {
                        //////根据斜率和四个中间值进行插值求解  
                        g1 = M[idx-W-1];  
                        g2 = M[idx-W];  
                        g3 = M[idx+W];  
                        g4 = M[idx+W+1];  
                        dWeight = fabs(P[idx])/fabs(Q[idx]);   //反正切  
                        dTmp1 = g1*dWeight+g2*(1-dWeight);
                        dTmp2 = g4*dWeight+g3*(1-dWeight);
                    }  
                    ////////////////////第二种情况///////////////////////  
                    /////////       g1                      /////////////  
                    /////////       g2  C   g3              /////////////  
                    /////////               g4              /////////////  
                    /////////////////////////////////////////////////////  
                    else if( ((Theta[idx]>=135)&&(Theta[idx]<180)) ||   
                        ((Theta[idx]>=315)&&(Theta[idx]<360))) {  
                        g1 = M[idx-W-1];  
                        g2 = M[idx-1];  
                        g3 = M[idx+1];  
                        g4 = M[idx+W+1];  
                        dWeight = fabs(Q[idx])/fabs(P[idx]);   //正切  
                        dTmp1 = g2*dWeight+g1*(1-dWeight);  
                        dTmp2 = g4*dWeight+g3*(1-dWeight);  
                    }  
                    ////////////////////第三种情况///////////////////////  
                    /////////           g1  g2              /////////////  
                    /////////           C                   /////////////  
                    /////////       g4  g3                  /////////////  
                    /////////////////////////////////////////////////////  
                    else if( ((Theta[idx]>=45)&&(Theta[idx]<90)) ||   
                        ((Theta[idx]>=225)&&(Theta[idx]<270))) {  
                        g1 = M[idx-W];  
                        g2 = M[idx-W+1];  
                        g3 = M[idx+W];  
                        g4 = M[idx+W-1];  
                        dWeight = fabs(P[idx])/fabs(Q[idx]);   //反正切  
                        dTmp1 = g2*dWeight+g1*(1-dWeight);  
                        dTmp2 = g3*dWeight+g4*(1-dWeight);  
                    }  
                    ////////////////////第四种情况///////////////////////  
                    /////////               g1              /////////////  
                    /////////       g4  C   g2              /////////////  
                    /////////       g3                      /////////////  
                    /////////////////////////////////////////////////////  
                    else if( ((Theta[idx]>=0)&&(Theta[idx]<45)) ||   
                        ((Theta[idx]>=180)&&(Theta[idx]<225))) {  
                        g1 = M[idx-W+1];  
                        g2 = M[idx+1];  
                        g3 = M[idx+W-1];
                        g4 = M[idx-1];  
                        dWeight = fabs(Q[idx])/fabs(P[idx]);   //正切  
                        dTmp1 = g1*dWeight+g2*(1-dWeight);  
                        dTmp2 = g3*dWeight+g4*(1-dWeight);
                    }
                    //////////进行局部最大值判断，并写入检测结果////////////////
                    ls.r(i, j) = ((M[idx]>=dTmp1) && (M[idx]>=dTmp2)) ? 128 : 0;
                }

            }
            
        }
        
        if(DEBUG) {
            cerr << "loose ready!" << endl;
            ls.draw("loose.bmp");
        }
        
        // 5. 双阈值检测 
        int nHist[1024] = {};
        int nEdgeNum;             //可能边界数  
        int nMaxMag = 0;          //最大梯度数  
        int nHighCount;
        
        // 统计直方图 
        for(i=0; i<W; i++) {  
            for(j=0; j<H; j++) {  
                if(ls.r(i, j) == 128) {
                    nHist[M[i+j*W]]++; 
                } 
            }
        }

        // 获取最大梯度幅值及潜在边缘点个数 
        nEdgeNum = nHist[0];
        //获取最大的梯度值
        nMaxMag = 0;                    
        for(i=1; i<1024; i++) {
            nMaxMag = nHist[i] ? i : nMaxMag;
            nEdgeNum += nHist[i];   //经过non-maximum suppression后有多少像素  
        }
        
        // 限制最大的边缘点数不能超过25% 
        int maxEdgeNum = W*H/4;
        for(i=1; i<1024 && nEdgeNum>=maxEdgeNum; ++i) {
            nEdgeNum -= nHist[i];
            nHist[i] = 0;
        }
        
        if(DEBUG) {
            cerr << "statistic ready! nEdgeNum = " << nEdgeNum  << ", nMaxMag = " << nMaxMag << endl;
        }

        // 计算两个阈值
        double  dRatHigh = 0.79;
        int dThrHigh;  
        int dThrLow;  
        double  dRatLow = 0.5;
        
        if(folder == "Lab2") {
            dRatHigh = 0.7;
        }
        
        nHighCount = (int)(dRatHigh * nEdgeNum + 0.5);  

        nEdgeNum = nHist[j=1];
        while((j<(nMaxMag-1)) && (nEdgeNum < nHighCount)) {
            nEdgeNum += nHist[++j];  
        }  
        dThrHigh = j;                                   //高阈值  
        dThrLow = (int)((dThrHigh) * dRatLow + 0.5);    //低阈值  
        
        if(DEBUG) {
            cerr << "threshold ready! thigh = " << dThrHigh << ", tlow = " << dThrLow << endl;
        }

        // 进行边缘检测
        for(i=0; i<W; i++) {
            for(j=0; j<H; j++) {
                idx = i+j*W;
                if((ls._R[idx]==128) && (M[idx] > dThrHigh)) {
                    ls._R[idx] = 255;
					// 跟踪边缘
					deque<int> qx, qy;
					qx.push_back(x);
					qy.push_back(y);
					while(!qx.empty()) {
						int x = qx.front(); qx.pop_front();
						int y = qy.front(); qy.pop_front();

					}
					for(int k=0, idx; k<8; k++) {
						y += dy[k];
						x += dx[k];
						idx = x + y * W;
						if(inside(x,y) && ls._R[idx]==128 && M[idx]>=dThrLow) {  
							//该点设为边界点  
							ls._R[idx] = 255;  
							//以该点为中心再进行跟踪
							qx.push_back(x);
							qy.push_back(y);
						}
						y -= dy[k];
						x -= dx[k];
					}
                }
            }  
        }
        
        if(DEBUG) {
            ls.draw("ls.bmp");
            cerr << "trace complete!!" << endl;
        }

        //将还没有设置为边界的点设置为非边界点  
        for(i=0; i<W; i++) {  
            for(j=0; j<H; j++) {  
                ls.r(i,j) = ls.r(i,j) == 255 ? 255 : 0;
            }  
        }
        
        // 删除七不沾的点点
        for(i=1; i<W-1; i++) {  
            for(j=1; j<H-1; j++) {
                int cnt = 0;
                cnt += ls.r(i,j+1) == 255;
                cnt += ls.r(i,j-1) == 255;
                cnt += ls.r(i-1,j+1) == 255;
                cnt += ls.r(i-1,j) == 255;
                cnt += ls.r(i-1,j-1) == 255;
                cnt += ls.r(i+1,j+1) == 255;
                cnt += ls.r(i+1,j) == 255;
                cnt += ls.r(i+1,j-1) == 255;
                if(cnt < 1) ls.r(i,j) = 0;
            }  
        }; 
        
        if(DEBUG) {
            ls.draw("canny.bmp");
            cerr << "canny complete!!" << endl;
        }
        
        delete[] P;
        delete[] Q;
        delete[] M;
        delete[] Theta;
        
        return ls;

    }
    
	// 在当前的图上描一个点，给出点坐标、半径以及 rgb 色彩值。
    void drawPoint(const Point& p, int range=5
            , int rr = 0xFF, int gg = 0, int bb = 0) {
        
        int x = int(p.x), y = int(p.y);
        
        for(int i = max(x-range, 0); i <= x+range && i < W; ++i) {
            for(int j = max(y-range, 0); j <= y+range && j < H; ++j) {
                if(rr>-1) r(i,j) = rr;
                if(gg>-1) g(i,j) = gg;
                if(bb>-1) b(i,j) = bb;
            }    
        }
    }
    
	// 在当前的图上描一条线
    void drawLine(const Line& line, int rr=0xFF, int gg=0, int bb=0) {
        
        Point p(-1, -1);
        
        // 找到边界交点 
        p = inside(p) ? p : intersect(line, Line(1, 0, 2));
        p = inside(p) ? p : intersect(line, Line(0, 1, 2));
        p = inside(p) ? p : intersect(line, Line(1, 0, 2-W));
        p = inside(p) ? p : intersect(line, Line(0, 1, 2-H));
        
        // 方向向量 
        Point d(-line.B, line.A);
        d /= d.length();
        
        while(inside(p+d)) drawPoint((p+=d), 1, rr, gg, bb);
        while(inside(p-d)) drawPoint((p-=d), 1, rr, gg, bb);

    }
    
	// 在当前的图上描一条线段 
    void drawLine(const Segment& seg, int rr=0, int gg=0, int bb=0xFF) {
        
        Point p = seg.a;
        
        // 方向向量 
        Point d = seg.b - seg.a;
        d /= d.length();
        while(dist(p, seg) < 2) {
            drawPoint(p, 1, rr, gg, bb);
            p += d;
        }

    }

    Point findRed() {

        if(DEBUG) {
            cerr << "find red start!!" << endl;
        }
        
        // 每个图像点的特征值 
        int* val = new int[W*H];
        
        // 每个图像点的特征累加（用于计算区域和） 
        int* acc = new int[W*H];
        
        int i, j, idx, tmp, h;
        
        // 计算每个点的特征值及其累加 
        // 特征值取色相角与纯红的夹角，小值更优 
        for(i = 0; i < W; ++i) {
            for(j = 0; j < H; ++j) {
                idx = i+j*W;
				h = hue(_R[idx], _G[idx], _B[idx]);
				if(h == -1 || int(_G[idx])+int(_B[idx]) > _R[idx]) val[idx] = 180;
				else val[idx] = min(h, 360-h);
				if(val[idx] < 20) val[idx] = 0;
                acc[idx] = val[idx];
                if(i) acc[idx] += acc[(i-1)+(j)*W];
                if(j) acc[idx] += acc[(i)+(j-1)*W];
                if(i&&j) acc[idx] -= acc[(i-1)+(j-1)*W];
            } 
        } 
        
        if(DEBUG) { 
            Image r(W,H);
            for(i = 0; i < W; ++i) {
                for(j = 0; j < H; ++j) {
                    r.r(i,j) = val[i+j*W];
                }
            }
            r.draw("red.bmp");
            cerr << "characterization finished!" << endl;
        }

        // 查找最优区域点 
        int best = INT_MAX, x = -1, y = -1;
        // 红色块区范围取到宽度的百分之一 
        int rng = W/80;
        for(i=rng; i<W-rng; ++i) {
            for(j=rng; j<H-rng; ++j) {
                
                if(j > H*2/3) break;
                
                tmp = acc[(i+rng)+(j+rng)*W]
                    - acc[(i+rng)+(j-rng)*W]
                    - acc[(i-rng)+(j+rng)*W]
                    + acc[(i-rng)+(j-rng)*W];
                // 差不多的话尽量把红点下移。 
                if(tmp < best || tmp < best+rng*rng*10 && i>x) {
                    best = min(best, tmp);
                    x = i;
                    y = j;
                }
            }
        }
        
        // 清理 
        delete [] val;
        delete [] acc;
        
        return Point(x, y);
    }
    
    // 通过霍夫变换输出给定边缘图在一定范围内识别到的直线 
    // d 为 true 水平 / false 垂直 
    vector<Segment> getLines(int w1, int w2, int h1, int h2, bool d
            , Image& img, Point red) {
        
        vector<Segment> ret(0);
        
        int i, j, k, rr, cc, mx, cnt, acc;
        
        // 参数设置 
        int hcnt, rng;
        
        if(folder == "Sim") {
            hcnt = 10;
            rng = 10;
        }
        else if(folder == "Lab") {
            hcnt = 10;
            rng = 10;
        }
        else if(folder == "Lab2") {
            hcnt = 10;
            rng = 20;
        }
        else if(folder == "ISS") {
            hcnt = 10;
            rng = 10;
        }
        else {
            hcnt = 10;
            rng = 10;
        }
        
        // 角度的精度倍数 
        const int A = 4, C = 360*A; 
        
        double _sin[C];
        double _cos[C];
        
        // 垂直/水平限制 
        int vc[C], vn = 0;
        bool b[C] = {};
        bool hor, ver;
        
        // preload triangle functions.
        for(cc = 0; cc < C; ++cc) {
            _sin[cc] = sin(cc * PI / 180 / A);
            _cos[cc] = cos(cc * PI / 180 / A);
            hor =   (cc>=35*A && cc<145*A) ||
                    (cc>=215*A && cc<325*A);
            ver =   (cc>=0*A && cc<55*A) || 
                    (cc>=125*A && cc<235*A) || 
                    (cc>=305*A && cc<360*A);
            if(!d && hor || d && ver) {
                vc[vn++] = cc;
                b[cc] = true;
            }
        }
        
        // 变换域的 size = R * C
        int R = int(sqrt(double(W*W+H*H)));

        int *_H = new int[R*C];
        
        for(i = w1; i < w2; ++i) {
            for(j = h1; j < h2; ++j) {
                // 当边缘图 (this) 在某点 (i,j) 为边缘时
                if(r(i, j) == 255) {
                    for(k = 0; k < vn; ++k) {
                        cc = vc[k];
                        rr = int(i * _cos[cc] + j * _sin[cc]);
                        if(rr >= 0 && rr < R) ++_H[cc*R+rr];
                    }
                }
            }
        }
        
        Image hf(C, R, true);
        
        if(DEBUG) {
            
            cerr << "hough complete!!" << endl;
            
            for(cc = mx = 0; cc < C; ++cc) {
                for(rr = 0; rr < R; ++rr) {
                    mx = max(mx, _H[cc*R+rr]);
                }
            }
            for(cc = 0; cc < C; ++cc) {
                for(rr = 0; rr < R; ++rr) {
                    hf.r(cc, rr) = _H[cc*R+rr] * 0xFF / mx;
                }
            }
        }
        
        if(folder == "Lab2") {
            // 对霍夫阵进行差分运算
            for(i = 0; i < C; ++i) {
                if(!b[i]) continue;
                for(j = 0; j < R; ++j) {
                    if(j==R-1||i==C-1||!b[i+1]) {
                        _H[i*R+j] = 0;
                    }
                    else {
                        _H[i*R+j] = abs(_H[i*R+j]*2 - _H[i*R+j+1] - _H[(i+1)*R+j]);
                    }
                } 
            } 
        }
        
        // 查找霍夫点
        int idx, val;
        
        vector<Segment> raw(0);
        
        while(raw.size() < hcnt) {
            mx = 0;
            int mc = -1, mr = -1;
            for(k = 0; k < vn; ++k) {
                cc = vc[k];
                for(rr = 0; rr < R; ++rr) {
                    val = _H[cc*R+rr];
                    if(mx < val) {
                        mx = val;
                        mc = cc;
                        mr = rr;
                    }
                }
            }
            Line l(_cos[mc], _sin[mc], -mr);
            raw.push_back(getSegment(l,w1,w2,h1,h2));

            if(DEBUG) { 
                //img.drawLine(l,0,0,0xFF);
                //img.drawLine(raw.back(),0,0,0xFF);
            }
            for(i=mc-rng; i<mc+rng; ++i) {
                if(i<0 || i>=C || !b[i]) continue;
                for(j=mr-rng; j<mr+rng; ++j) {
                    if(j<0 || j>=R) continue;
                    _H[i*R+j] = 0;
                }
            }
        }
        
        if(DEBUG) {
            cerr << "hough detected!!" << endl;
            
            for(cc = 0; cc < C; ++cc) {
                for(rr = 0; rr < R; ++rr) {
                    hf.g(cc, rr) = hf.b(cc, rr) = _H[cc*R+rr] * 0xFF / mx;
                }
            }
            
            hf.draw(d?"hf_hor.bmp":"hf_ver.bmp");

            cerr << "hough printed!!" << endl;
        }
        
        sort(raw.rbegin(), raw.rend());
        
        if(raw.size() > 0) {
            for(i = 0; i < raw.size(); ++i) {
                // 筛选长度为最长的 1/6 的线段。
                // 除非线段靠着边 
                if(raw[i].length() < raw[0].length() / 6 &&
                    !(abs(raw[i].a.x - w1) < 2 || 
                    abs(raw[i].a.x - w2) < 2 || 
                    abs(raw[i].b.x - w1) < 2 || 
                    abs(raw[i].b.x - w2) < 2 ||
                    abs(raw[i].a.y - h1) < 2 || 
                    abs(raw[i].a.y - h2) < 2 || 
                    abs(raw[i].b.y - h1) < 2 || 
                    abs(raw[i].b.y - h2) < 2 )
                ) continue; 
//img.drawLine(raw[i], 0xFF, -1, -1);
                ret.push_back(raw[i]);
                if(DEBUG) {
                    cerr<<raw[i].length()<<endl;
                    img.drawLine(raw[i], 0xFF, 0, 0);
                    //cerr << cc << " " << rr << " " << len << endl;
                }
            }
        }
        
        delete [] _H;
        
        return ret;
    }
    
    vector<Point> getVertex() {
        
        
        // Edge image.
        Image e = getEdge();
        
        // return vertex, up-left, up-right, down-left, down-right.
        vector<Point> ret(4);
        
        // 4-direction bounding line.
        Line lu(0,1,0), ld(0,1,-H), ll(0,1,0), lr(1,0,-W);
        
        // red point, garantee to be inside.
        Point red = findRed();
        
        if(DEBUG) {
            cerr << "red point found! " << red <<  endl;
        }
        
        // stripe bounding, red point to be center, width at most H/2 or W/4.
        int h1, h2, w1, w2;
        if(folder == "Sim") {
            h1 = max(0, int(red.y) - H/8);
            h2 = min(H-1, int(red.y) + H*3/5);
            w1 = max(0, int(red.x) - W/3);
            w2 = min(W-1, int(red.x) + W/4);
        }
        else if(folder == "Lab") {
            h1 = max(0, int(red.y) - H/8);
            h2 = min(H-1, int(red.y) + H*3/8);
            w1 = max(0, int(red.x) - W/3);
            w2 = min(W-1, int(red.x) + W/6);
        }
        else if(folder == "Lab2") {
            h1 = max(0, int(red.y) - H/8);
            h2 = min(H-1, int(red.y) + H*3/8);
            w1 = max(0, int(red.x) - W/3);
            w2 = min(W-1, int(red.x) + W/6);
        }
        else {
            h1 = max(0, int(red.y) - H/8);
            h2 = min(H-1, int(red.y) + H*3/5);
            w1 = max(0, int(red.x) - W/3);
            w2 = min(W-1, int(red.x) + W/4);
        }
        
        if(DEBUG) {
            // render
            for(int i = w1; i < w2; ++i) {
                for(int j = 0; j < H; ++j) {
                    b(i,j) = 64;
                }
            }
            for(int i = 0; i < W; ++i) {
                for(int j = h1; j < h2; ++j) {
                    r(i,j) = 64;
                }
            }
        }
        
        // get horizontal lines.
        if(DEBUG) {
            cerr << "start line detection! " <<  endl;
        }
        
        // horizontal line detection
        vector<Segment> hl = e.getLines(w1, w2, 0, H, false, *this, red);
        
        // vertival line detection
        vector<Segment> vl = e.getLines(0, W, h1, h2, true, *this, red);
        
        // get up/down line.
        double maxy = -9999, miny = 9999;
        for(int i = 0; i < hl.size(); ++i) {
            double yy = intersect(Line(hl[i]), Line(1, 0, -red.x)).y;
            
            if(DEBUG) {
            //drawLine(hl[i], -1,0xFF,-1);
            //drawLine(Line(1, 0, -red.x+H/4), 255,255,255);
            }
            if(!cross(hl[i], Line(1, 0, -red.x+H/30))
                && !cross(hl[i], Line(1, 0, -red.x-H/20))
                && !cross(hl[i], Line(1, 0, -w2+5))
                && !((red.x > W*5/6 || folder=="Lab2") && cross(hl[i], Line(1, 0, -red.x+H/4)))
                ) continue;
            if(red.x < W/6 && 
                fabs(hl[i].a.y-hl[i].b.y) > fabs(hl[i].a.x-hl[i].b.x)*0.6) continue;
                
            if(DEBUG) {
            drawLine(hl[i], -1,-1,0xFF);
            }
            if(yy < red.y && dist(red, Line(hl[i])) > H/40 && yy > maxy) {
                if(!(folder == "Lab2" && dist(red, Line(hl[i])) < H/17)){ 
                lu = hl[i];
                maxy = yy;
                }
            }
            if(yy > red.y && dist(red, Line(hl[i])) > H/5 && yy < miny) {
                ld = hl[i];
                miny = yy;
            }
        }
        
        // get left/right line.
        double maxx = -9999, minx = 9999;
        for(int i = 0; i < vl.size(); ++i) {
            //drawLine(vl[i], 0xFF,0xFF,-1);
            if(!(red.x < W/10) &&
                !cross(vl[i], Line(0, 1, -red.y-H/10)) &&
                //!cross(vl[i], Line(0, 1, -red.y+H/20)) &&
                !cross(vl[i], Line(0, 1, -red.y-H/3)) &&
                !cross(vl[i], Line(0, 1, -h2+5))) continue;
            
            double kk = eq(vl[i].a.x,vl[i].b.x) ?
                    1e9 : (vl[i].a.y-vl[i].b.y)/(vl[i].a.x-vl[i].b.x);
            
            if(folder == "Lab2" && fabs(kk) < 1.2) continue;
            
            if(DEBUG) {
                drawLine(vl[i], -1,-1,0xFF);
                //drawLine(Line(0, 1, -h2+5), 0xFF,0xFF,0xFF);
            }
            
            double xx = intersect(vl[i], Line(0, 1, -red.y)).x;
            if(!(kk<0 && kk>-1.732) && // 左边线不得右倾30度以上 
                    xx < red.x && dist(red, Line(vl[i])) > H/10 &&
                    xx > maxx) {
                ll = vl[i];
                maxx = xx;
            }
            if(!(kk>0 && kk<1.732) && // 右边线不得左倾30度以上 
                    xx > red.x && dist(red, Line(vl[i])) > H/20 && xx < minx) {
                lr = vl[i];
                minx = xx;
            }
        }
        
        if(DEBUG) {
            //*
            drawLine(lu, -1, 0xFF, -1);
            drawLine(ld, -1, 0xFF, -1);
            drawLine(ll, 0xFF, -1, 0xFF);
            drawLine(lr, 0xFF, -1, 0xFF);
            //*/
        }
        
        
        // output the image
        if(DEBUG) {
            // mark redPiont
            drawPoint(red, W/100, 0, 0, 0); 
            draw();
        }
        
        ret[0] = intersect(lu, ll);
        ret[1] = intersect(lu, lr);
        ret[2] = intersect(ld, ll);
        ret[3] = intersect(ld, lr);
        
        return ret;
    }
    
    void getStatus() {
        
        // 估计的面板面积 
        double s = area(vv[0], vv[1], vv[3])
                + area(vv[0], vv[2], vv[3]);
                
        
    }
};



#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;


class RobonautEye {
    
    int W;
    int H;
    
    Point getPoint(double kx, double ky) {
        
        Point a = vv[0], b = vv[1], c = vv[2], d = vv[3];
        Line lh1(a,b), lh2(c,d), lv1(a,c), lv2(b,d);
        Point oh = intersect(lh1, lh2);
        Point ov = intersect(lv1, lv2);
        bool hp = parallel(lh1, lh2);
        bool vp = parallel(lv1, lv2);
        double tx1 = 0, tx2 = 1, ty1 = 0, ty2 = 1;
        while(dist(a,b) > 0.1) {
            Point o = intersect(Line(a,d), Line(b,c));
//img.drawPoint(o,2);
            // 要处理平行线的情况 
            Line mh = hp ? Line(lh1.A,lh1.B,lh1.C-lh1.eval(o)) : Line(oh, o); 
            Line mv = vp ? Line(lv1.A,lv1.B,lv1.C-lv1.eval(o)) : Line(ov, o);
            double tx = (tx1+tx2)/2, ty = (ty1+ty2)/2;
//cerr << "tx = " << tx << ", ty = " << ty << " " <<o << endl;
            if(kx > tx) {
                lv1 = mv;
                tx1 = tx;
            }
            else {
                lv2 = mv;
                tx2 = tx;
            }
            if(ky > ty) {
                lh1 = mh;
                ty1 = ty;
            }
            else {
                lh2 = mh;
                ty2 = ty;
            }
            a = intersect(lh1, lv1);
            b = intersect(lh1, lv2);
            c = intersect(lh2, lv1);
            d = intersect(lh2, lv2);
        }
//img.draw("trace.bmp");
        return (a+b)*0.5;
    }
    
    vector<string> ans;

public:
    
    vector <string> recognizeObjects(
            const vector <int> &limg, 
            const vector <int> &rimg) {
        
        H = limg[0];
        W = limg[1];
        
        // answer
        for(int i = 0; i < 22; ++i) {
            X1[i] = X2[i] = W/2;
            Y1[i] = Y2[i] = H/2;
            S[i] = st[i];
        }
        
        double scale = 1;
        
        Image img1(limg), img2(rimg);
        
        while(W > 2000) {
            scale *= 2;
            img1.shrink();
            img2.shrink();
            H /= 2;
            W /= 2;
            
            if(DEBUG) {
                cerr << "shrink ready!!" << endl;
            }
        }

        // 根据噪声情况决定是否滤波 
        diff = 0;

        for(int i = 1; i < img1.W; ++i) {
            for(int j = 1; j < img1.H; ++j) {
                int rr = 0 + img1.r(i,j) + img1.r(i,j) - img1.r(i,j-1) - img1.r(i-1,j);
                int gg = 0 + img1.g(i,j) + img1.g(i,j) - img1.g(i,j-1) - img1.g(i-1,j);
                int bb = 0 + img1.b(i,j) + img1.b(i,j) - img1.b(i,j-1) - img1.b(i-1,j);
                diff += sqrt(rr*rr+gg*gg+bb*bb);
            } 
        }
        diff /= (W-1)*(H-1);
        
        if(diff < 8) folder = "Sim";
        else if(diff < 15) folder = "Lab"; 
        else if(diff < 30) folder = "Lab2";
        if(limg[0] == 2448 && limg[1] == 2050) folder = "ISS";
        
        if(folder == "Lab2") {
            for(int i = 0; i < 22; ++i) {
                PS[i][0] = 1.0 - (1.0-PS[i][0])*0.97;
            }
        }
        
        if(DEBUG) {
            cerr << diff << endl;
        }
//*
        if(folder != "Sim") img1.getFilted();
        vv = img1.getVertex();
        for(int i = 0; i < 22; ++i) {
            Point p = getPoint(PS[i][0], PS[i][1]);
            X1[i] = int(p.x*scale+0.5);
            Y1[i] = int(p.y*scale+0.5);
            if(!img1.inside(p)) {
                X1[i] = int(W*scale/2);
                Y1[i] = int(H*scale/2);
            }
        }
        if(DEBUG) img1.draw("img1.bmp");
//*/
//*
        if(folder != "Sim") img2.getFilted();
        vv = img2.getVertex();
        for(int i = 0; i < 22; ++i) {
            Point p = getPoint(PS[i][0], PS[i][1]);
            X2[i] = int(p.x*scale+0.5);
            Y2[i] = int(p.y*scale+0.5);
            if(!img2.inside(p)) {
                X2[i] = int(W*scale/2);
                Y2[i] = int(H*scale/2);
            }
        }
        if(DEBUG) img2.draw("img2.bmp");
//*/
        ans.assign(22, "");
        
        for(int i = 0; i < 22; ++i) {
            sprintf(sbuf,"%s,%d,%d,%d,%d"
                    ,S[i].c_str()
                    , X1[i], Y1[i], X2[i], Y2[i]);
            if(DEBUG) {
                cerr << sbuf << endl;
            }
            ans[i] = sbuf;
        }
        
        return ans;
        
    }
};

RobonautEye re;

int main() {
    //
//    Image img(80, 160, true);
//    
//    for(int x = 0; x < 20; ++x) {
//        for(int y = 0; y < 40; ++y) {
//            img.r(x, y) = 0xFF;
//            img.g(x, y) = 0xFF;
//        }
//    }
//    
    //img.draw("test.bmp");

    int len;
    scanf("%d", &len);
    vector<int> limg(len), rimg(len);
    for(int i = 0; i < len; ++i) scanf("%d", &limg[i]);
    for(int i = 0; i < len; ++i) scanf("%d", &rimg[i]);
    vector<string> ret = re.recognizeObjects(limg, rimg);
    for(size_t i = 0; i < ret.size(); ++i) {
        cout << ret[i] << endl;
    }
}
