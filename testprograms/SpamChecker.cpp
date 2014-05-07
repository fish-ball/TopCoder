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

class SpamChecker {
public:
	string spamCheck(string, int, int);
};

string SpamChecker::spamCheck(string judgeLog, int good, int bad) {
	int x = 0;
	for(int i = 0; i < judgeLog.size(); ++i) {
		if(judgeLog[i] == 'o') x += good;
		else x -= bad;
		if(x < 0) return "SPAM";
	}
	return "NOT SPAM";
}


double test0() {
	string p0 = "ooooxxxo";
	int p1 = 2;
	int p2 = 3;
	SpamChecker * obj = new SpamChecker();
	clock_t start = clock();
	string my_answer = obj->spamCheck(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p3 = "SPAM";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p3 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
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
	string p0 = "ooooxxxo";
	int p1 = 3;
	int p2 = 4;
	SpamChecker * obj = new SpamChecker();
	clock_t start = clock();
	string my_answer = obj->spamCheck(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p3 = "NOT SPAM";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p3 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
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
	string p0 = "xooooooooooooooooooooooooooo";
	int p1 = 1000;
	int p2 = 1;
	SpamChecker * obj = new SpamChecker();
	clock_t start = clock();
	string my_answer = obj->spamCheck(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p3 = "SPAM";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p3 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
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
	string p0 = "oxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	int p1 = 1000;
	int p2 = 1;
	SpamChecker * obj = new SpamChecker();
	clock_t start = clock();
	string my_answer = obj->spamCheck(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p3 = "NOT SPAM";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p3 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
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
	string p0 = "ooxoxoxooxoxxoxoxooxoxoxoxxoxx";
	int p1 = 15;
	int p2 = 17;
	SpamChecker * obj = new SpamChecker();
	clock_t start = clock();
	string my_answer = obj->spamCheck(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p3 = "SPAM";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p3 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p3 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test5() {
	string p0 = "oooxoxoxoxoxoxooxooxoxooxo";
	int p1 = 16;
	int p2 = 18;
	SpamChecker * obj = new SpamChecker();
	clock_t start = clock();
	string my_answer = obj->spamCheck(p0, p1, p2);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p3 = "NOT SPAM";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p3 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
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
	
	time = test5();
	if (time < 0)
		errors = true;
	
	if (!errors)
		cout <<"You're a stud (at least on the example cases)!" <<endl;
	else
		cout <<"Some of the test cases had errors." <<endl;
}

//Powered by [KawigiEdit] 2.0!
