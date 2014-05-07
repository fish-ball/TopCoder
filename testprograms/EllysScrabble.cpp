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

class EllysScrabble {
public:
	string getMin(string, int);
};



string EllysScrabble::getMin(string letters, int maxDistance) {
	
	int n = letters.size();
	vector<int> o(n);
	for(int i = 0; i < n; ++i) o[i] = i;
	

	for(int i = 0; i < n; ++i) {
		int p = i;
		
		
		//cout << "---" << i << endl;
		for(int j = i + 1; j < n; ++j) {
		//cout << "  -" << j << letters[j] << letters[p] <<  endl;
			if(letters[j] < letters[p] && abs(o[j]-i)<=maxDistance && abs(o[i]-j)<=maxDistance) {
				p = j;
		//cout << p << endl;
			}
		}
		swap(letters[i], letters[p]);
		swap(o[i], o[p]);
		
		for(int j = i + 1; j < n; ++j) {
			for(int k = j + 1; k < n; ++k) {
				if(o[k] > o[j] && abs(o[k]-j)<=maxDistance && abs(o[j]-k)<=maxDistance) {
					swap(letters[j], letters[k]);
					swap(o[j], o[k]);
				}
			}
		}
	}
	return letters;
}


double test0() {
	string p0 = "TOPCODER";
	int p1 = 3;
	EllysScrabble * obj = new EllysScrabble();
	clock_t start = clock();
	string my_answer = obj->getMin(p0, p1);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p2 = "CODTEPOR";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p2 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p2 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test1() {
	string p0 = "ESPRIT";
	int p1 = 3;
	EllysScrabble * obj = new EllysScrabble();
	clock_t start = clock();
	string my_answer = obj->getMin(p0, p1);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p2 = "EIPRST";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p2 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p2 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test2() {
	string p0 = "BAZINGA";
	int p1 = 8;
	EllysScrabble * obj = new EllysScrabble();
	clock_t start = clock();
	string my_answer = obj->getMin(p0, p1);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p2 = "AABGINZ";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p2 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p2 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test3() {
	string p0 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int p1 = 9;
	EllysScrabble * obj = new EllysScrabble();
	clock_t start = clock();
	string my_answer = obj->getMin(p0, p1);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p2 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p2 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test4() {
	string p0 = "GOODLUCKANDHAVEFUN";
	int p1 = 7;
	EllysScrabble * obj = new EllysScrabble();
	clock_t start = clock();
	string my_answer = obj->getMin(p0, p1);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p2 = "CADDGAHEOOFLUKNNUV";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p2 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p2 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test5() {
	string p0 = "AAAWDIUAOIWDESBEAIWODJAWDBPOAWDUISAWDOOPAWD";
	int p1 = 6;
	EllysScrabble * obj = new EllysScrabble();
	clock_t start = clock();
	string my_answer = obj->getMin(p0, p1);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p2 = "AAAADDEIBWAEUIODWADSBIAJWODIAWDOPOAWDUOSPWW";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p2 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p2 != my_answer) {
		cout <<"DOESN'T MATCH!!!!" <<endl <<endl;
		return -1;
	}
	else {
		cout <<"Match :-)" <<endl <<endl;
		return (double)(end-start)/CLOCKS_PER_SEC;
	}
}
double test6() {
	string p0 = "ABRACADABRA";
	int p1 = 2;
	EllysScrabble * obj = new EllysScrabble();
	clock_t start = clock();
	string my_answer = obj->getMin(p0, p1);
	clock_t end = clock();
	delete obj;
	cout <<"Time: " <<(double)(end-start)/CLOCKS_PER_SEC <<" seconds" <<endl;
	string p2 = "AABARACBDAR";
	cout <<"Desired answer: " <<endl;
	cout <<"\t\"" << p2 <<"\"" <<endl;
	cout <<"Your answer: " <<endl;
	cout <<"\t\"" << my_answer<<"\"" <<endl;
	if (p2 != my_answer) {
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
