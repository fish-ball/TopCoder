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

class WolvesAndSheep {
public:
	int getmin(vector <string>);
};

int WolvesAndSheep::getmin(vector <string> field) {
	int N = field.size(), M = field[0].size();
	int ans = N + M - 2;
	char b[16][16];
	for(int xx = 0, cx; xx < (1<<N-1); ++xx) {
		cx = bitset<16>(xx).count();
		if(cx >= ans) continue;
		for(int yy = 0, cy; yy < (1<<M-1); ++yy) {
			cy = bitset<16>(yy).count();
			if(cy + cx >= ans) continue;
			memset(b, -1, sizeof(b));
			bool success = true;
			for(int i = 0; i < N && success; ++i) {
				for(int j = 0; j < M && success; ++j) {
					if(b[i][j] == -1) {
//if(N < 6) cout << i << ", " << j << endl;
						b[i][j] = 0;
						for(int ii = i; ii < N; ++ii) {
							for(int jj = j; jj < M&&jj<3; ++jj) {
								b[ii][jj] = 0;
								if(field[ii][jj] == 'W') b[i][j] |= 1;
								else if(field[ii][jj] == 'S') b[i][j] |= 2;
								if(yy&(1<<jj)) break;
							}
							if(xx&(1<<ii)) break;
						}
						if(b[i][j] == 3) success = false;
					}
				}
			}
			if(success) {
cout << bitset<16>(xx) << " " << bitset<16>(yy) << endl;
				ans = cx + cy;
			}
		}
	}
	return ans;
}


double test0() {
	string t0[] = {"W.WSS",
 "WW.S.",
 ".SSS.",
 "SSS.S",
 ".SS.S"};
	vector <string> p0(t0, t0+sizeof(t0)/sizeof(string));
	WolvesAndSheep * obj = new WolvesAndSheep();
	clock_t start = clock();
	int my_answer = obj->getmin(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 2;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test1() {
	string t0[] = {".....",
 ".....",
 "....."};
	vector <string> p0(t0, t0+sizeof(t0)/sizeof(string));
	WolvesAndSheep * obj = new WolvesAndSheep();
	clock_t start = clock();
	int my_answer = obj->getmin(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 0;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test2() {
	string t0[] = {".SS",
 "...",
 "S..",
 "W.W"};
	vector <string> p0(t0, t0+sizeof(t0)/sizeof(string));
	WolvesAndSheep * obj = new WolvesAndSheep();
	clock_t start = clock();
	int my_answer = obj->getmin(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 1;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test3() {
	string t0[] = {"WWWSWWSSWWW",
 "WWSWW.SSWWW",
 "WS.WSWWWWS."};
	vector <string> p0(t0, t0+sizeof(t0)/sizeof(string));
	WolvesAndSheep * obj = new WolvesAndSheep();
	clock_t start = clock();
	int my_answer = obj->getmin(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 10;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test4() {
	string t0[] = {".W.S.W.W",
 "W.W.S.W.",
 ".S.S.W.W",
 "S.S.S.W.",
 ".S.W.W.S",
 "S.S.W.W.",
 ".W.W.W.S",
 "W.W.S.S."};
	vector <string> p0(t0, t0+sizeof(t0)/sizeof(string));
	WolvesAndSheep * obj = new WolvesAndSheep();
	clock_t start = clock();
	int my_answer = obj->getmin(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 7;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test5() {
	string t0[] = {"W.SSWWSSSW.SS",
 ".SSSSW.SSWWWW",
 ".WWWWS.WSSWWS",
 "SS.WSS..W.WWS",
 "WSSS.SSWS.W.S",
 "WSS.WS...WWWS",
 "S.WW.S.SWWWSW",
 "WSSSS.SSW...S",
 "S.WWSW.WWSWSW",
 ".WSSS.WWSWWWS",
 "..SSSS.WWWSSS",
 "SSWSWWS.W.SSW",
 "S.WSWS..WSSS.",
 "WS....W..WSS."};
	vector <string> p0(t0, t0+sizeof(t0)/sizeof(string));
	WolvesAndSheep * obj = new WolvesAndSheep();
	clock_t start = clock();
	int my_answer = obj->getmin(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 24;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test6() {
	string t0[] = {"WW..SS",
 "WW..SS",
 "......",
 "......",
 "SS..WW",
 "SS..WW"};
	vector <string> p0(t0, t0+sizeof(t0)/sizeof(string));
	WolvesAndSheep * obj = new WolvesAndSheep();
	clock_t start = clock();
	int my_answer = obj->getmin(p0);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	int p1 = 2;
	cout <<"Desired answer: " <<endl;
	cout <<"\t" << p1 <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t" << my_answer <<endl;
	if (p1 != my_answer) {
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
	
	time = test5();
	if (time < 0)
		errors = true;
	
	time = test6();
	if (time < 0)
		errors = true;
	
	if (!errors)
		cout <<"You're a stud (at least on the example cases)!" <<endl;
	else
		cout <<"Some of the test cases had errors." <<endl;
}

//Powered by [KawigiEdit] 2.0!
