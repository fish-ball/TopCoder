#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

class MinimumSquare {
public:
	long long minArea(vector <int>, vector <int>, int);
};

typedef long long i64;

long long MinimumSquare::minArea(vector <int> x, vector <int> y, int K) {
	int n = x.size();
	i64 ans = 2000000005;
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			i64 x0 = min(x[i], x[j]) - 1;
			i64 y0 = min(y[i], y[j]) - 1;
			i64 l = 0, r = ans, m;
			while(l + 1 < r) {
				m = (l + r + 1) / 2;
				int k = 0;
				for(int p = 0; p < n; ++p) {
					if(x[p] > x0 && x[p] < x0 + m && y[p] > y0 && y[p] < y0 + m) {
						++k;
					}
				}
				if(k < K) l = m;
				else r = m;
			}
			ans = min(ans, r);
		}
	}
	return ans * ans;
	
}


double test0() {
	int t0[] = {0, 3};
	vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
	int t1[] = {0, 7};
	vector <int> p1(t1, t1+sizeof(t1)/sizeof(int));
	int p2 = 2;
	MinimumSquare * obj = new MinimumSquare();
	clock_t start = clock();
	long long my_answer = obj->minArea(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p3 = 81LL;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p3 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p3 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test1() {
	int t0[] = {-4, 3, 1};
	vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
	int t1[] = {3 , -1, -2};
	vector <int> p1(t1, t1+sizeof(t1)/sizeof(int));
	int p2 = 2;
	MinimumSquare * obj = new MinimumSquare();
	clock_t start = clock();
	long long my_answer = obj->minArea(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p3 = 16LL;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p3 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p3 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test2() {
	int t0[] = {0, 0, 1, 1, 2, 2};
	vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
	int t1[] = {0, 1, 0, 1, 0, 1};
	vector <int> p1(t1, t1+sizeof(t1)/sizeof(int));
	int p2 = 4;
	MinimumSquare * obj = new MinimumSquare();
	clock_t start = clock();
	long long my_answer = obj->minArea(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p3 = 9LL;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p3 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p3 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test3() {
	int t0[] = {1000000000, 1000000000, 1000000000, -1000000000, -1000000000, -1000000000};
	vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
	int t1[] = {1000000000, 0, -1000000000, 1000000000, 0, -1000000000};
	vector <int> p1(t1, t1+sizeof(t1)/sizeof(int));
	int p2 = 3;
	MinimumSquare * obj = new MinimumSquare();
	clock_t start = clock();
	long long my_answer = obj->minArea(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p3 = 4000000008000000004LL;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p3 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p3 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test4() {
	int t0[] = {-47881, 28623, 1769, -38328, -16737, 16653, -23181, 37360, 41429, 26282, 254, 728, 8299, -41080, -29498, 17488, -23937, -11, 33319, 25232};
	vector <int> p0(t0, t0+sizeof(t0)/sizeof(int));
	int t1[] = {-46462, 48985, -43820, -19587, -33593, -28337, 13667, -48131, -5568, -2332, -41918, -31370, -3695, 42599, -37788, -40096, 39049, 25045, -2122, 3874};
	vector <int> p1(t1, t1+sizeof(t1)/sizeof(int));
	int p2 = 8;
	MinimumSquare * obj = new MinimumSquare();
	clock_t start = clock();
	long long my_answer = obj->minArea(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	long long p3 = 1695545329LL;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p3 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p3 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}

int main() {
	int time;
	bool errors = false;
	
	time = test0();
	if (time < 0)
		errors = true;
	
	time = test1();
	if (time < 0)
		errors = true;
	
	time = test2();
	if (time < 0)
		errors = true;
	
	time = test3();
	if (time < 0)
		errors = true;
	
	time = test4();
	if (time < 0)
		errors = true;
	
	if (!errors)
		cout <<"You're a stud (at least on the example cases)!" <<endl;
	else
		cout <<"Some of the test cases had errors." <<endl;
}

//Powered by [KawigiEdit] 2.0!
